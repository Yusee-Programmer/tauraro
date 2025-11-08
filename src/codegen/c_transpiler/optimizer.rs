//! Code Optimization Passes for Native C Transpiler
//!
//! Implements various optimization techniques to improve generated C code performance

use crate::ast::*;

/// Optimizer for native C code generation
pub struct NativeOptimizer {
    constant_folding_enabled: bool,
    dead_code_elimination_enabled: bool,
    strength_reduction_enabled: bool,
}

impl NativeOptimizer {
    pub fn new() -> Self {
        Self {
            constant_folding_enabled: true,
            dead_code_elimination_enabled: true,
            strength_reduction_enabled: true,
        }
    }

    /// Optimize a program
    pub fn optimize_program(&self, program: &mut Program) {
        for stmt in &mut program.statements {
            self.optimize_statement(stmt);
        }
    }

    /// Optimize a statement
    fn optimize_statement(&self, stmt: &mut Statement) {
        match stmt {
            Statement::VariableDef { value, .. } => {
                if let Some(expr) = value {
                    *expr = self.optimize_expr(expr.clone());
                }
            }
            Statement::FunctionDef { body, .. } => {
                for stmt in body {
                    self.optimize_statement(stmt);
                }
            }
            Statement::If { condition, then_branch, elif_branches, else_branch } => {
                *condition = self.optimize_expr(condition.clone());
                for stmt in then_branch {
                    self.optimize_statement(stmt);
                }
                for (cond, stmts) in elif_branches {
                    for stmt in stmts {
                        self.optimize_statement(stmt);
                    }
                }
                if let Some(stmts) = else_branch {
                    for stmt in stmts {
                        self.optimize_statement(stmt);
                    }
                }
            }
            Statement::While { condition, body, .. } => {
                *condition = self.optimize_expr(condition.clone());
                for stmt in body {
                    self.optimize_statement(stmt);
                }
            }
            Statement::For { body, .. } => {
                for stmt in body {
                    self.optimize_statement(stmt);
                }
            }
            Statement::Return(Some(expr)) => {
                *expr = self.optimize_expr(expr.clone());
            }
            Statement::Expression(expr) => {
                *expr = self.optimize_expr(expr.clone());
            }
            Statement::Try { body, except_handlers, else_branch, finally } => {
                for stmt in body {
                    self.optimize_statement(stmt);
                }
                for handler in except_handlers {
                    for stmt in &mut handler.body {
                        self.optimize_statement(stmt);
                    }
                }
                if let Some(stmts) = else_branch {
                    for stmt in stmts {
                        self.optimize_statement(stmt);
                    }
                }
                if let Some(stmts) = finally {
                    for stmt in stmts {
                        self.optimize_statement(stmt);
                    }
                }
            }
            _ => {}
        }
    }

    /// Optimize an expression
    fn optimize_expr(&self, expr: Expr) -> Expr {
        match expr {
            // Constant folding for binary operations
            Expr::BinaryOp { left, op, right } if self.constant_folding_enabled => {
                let left_opt = self.optimize_expr(*left);
                let right_opt = self.optimize_expr(*right);

                // Try to fold constants
                if let (Expr::Literal(Literal::Int(a)), Expr::Literal(Literal::Int(b))) = (&left_opt, &right_opt) {
                    match op {
                        BinaryOp::Add => return Expr::Literal(Literal::Int(a + b)),
                        BinaryOp::Sub => return Expr::Literal(Literal::Int(a - b)),
                        BinaryOp::Mul => return Expr::Literal(Literal::Int(a * b)),
                        BinaryOp::Div if *b != 0 => return Expr::Literal(Literal::Int(a / b)),
                        BinaryOp::Mod if *b != 0 => return Expr::Literal(Literal::Int(a % b)),
                        _ => {}
                    }
                }

                // Strength reduction for multiplication/division by power of 2
                if self.strength_reduction_enabled {
                    if let Expr::Literal(Literal::Int(n)) = &right_opt {
                        match op {
                            BinaryOp::Mul => {
                                // Check if n is power of 2
                                if *n > 0 && (*n & (*n - 1)) == 0 {
                                    let shift = (*n as f64).log2() as i64;
                                    return Expr::BinaryOp {
                                        left: Box::new(left_opt),
                                        op: BinaryOp::LShift,
                                        right: Box::new(Expr::Literal(Literal::Int(shift))),
                                    };
                                }
                            }
                            BinaryOp::Div => {
                                // Check if n is power of 2
                                if *n > 0 && (*n & (*n - 1)) == 0 {
                                    let shift = (*n as f64).log2() as i64;
                                    return Expr::BinaryOp {
                                        left: Box::new(left_opt),
                                        op: BinaryOp::RShift,
                                        right: Box::new(Expr::Literal(Literal::Int(shift))),
                                    };
                                }
                            }
                            _ => {}
                        }
                    }
                }

                // Algebraic simplifications
                match (&left_opt, &op, &right_opt) {
                    // x + 0 = x
                    (_, BinaryOp::Add, Expr::Literal(Literal::Int(0))) => return left_opt,
                    (Expr::Literal(Literal::Int(0)), BinaryOp::Add, _) => return right_opt,

                    // x - 0 = x
                    (_, BinaryOp::Sub, Expr::Literal(Literal::Int(0))) => return left_opt,

                    // x * 0 = 0
                    (_, BinaryOp::Mul, Expr::Literal(Literal::Int(0))) => return Expr::Literal(Literal::Int(0)),
                    (Expr::Literal(Literal::Int(0)), BinaryOp::Mul, _) => return Expr::Literal(Literal::Int(0)),

                    // x * 1 = x
                    (_, BinaryOp::Mul, Expr::Literal(Literal::Int(1))) => return left_opt,
                    (Expr::Literal(Literal::Int(1)), BinaryOp::Mul, _) => return right_opt,

                    // x / 1 = x
                    (_, BinaryOp::Div, Expr::Literal(Literal::Int(1))) => return left_opt,

                    _ => {}
                }

                Expr::BinaryOp {
                    left: Box::new(left_opt),
                    op,
                    right: Box::new(right_opt),
                }
            }

            // Constant folding for unary operations
            Expr::UnaryOp { op, operand } if self.constant_folding_enabled => {
                let operand_opt = self.optimize_expr(*operand);

                if let Expr::Literal(Literal::Int(n)) = &operand_opt {
                    match op {
                        UnaryOp::USub | UnaryOp::Minus => return Expr::Literal(Literal::Int(-n)),
                        UnaryOp::UAdd => return operand_opt,
                        _ => {}
                    }
                }

                if let Expr::Literal(Literal::Bool(b)) = &operand_opt {
                    if let UnaryOp::Not = op {
                        return Expr::Literal(Literal::Bool(!b));
                    }
                }

                Expr::UnaryOp {
                    op,
                    operand: Box::new(operand_opt),
                }
            }

            // Recursively optimize list comprehensions
            Expr::ListComp { element, generators } => {
                Expr::ListComp {
                    element: Box::new(self.optimize_expr(*element)),
                    generators,
                }
            }

            // Recursively optimize other expressions
            Expr::Call { func, args, kwargs } => {
                let optimized_args = args.into_iter()
                    .map(|arg| self.optimize_expr(arg))
                    .collect();
                Expr::Call {
                    func,
                    args: optimized_args,
                    kwargs,
                }
            }

            // Return expr as-is if no optimization applies
            _ => expr,
        }
    }

    /// Dead code elimination - remove unreachable statements
    pub fn eliminate_dead_code(&self, statements: &mut Vec<Statement>) {
        if !self.dead_code_elimination_enabled {
            return;
        }

        let mut reachable = true;
        statements.retain(|stmt| {
            if !reachable {
                return false; // Remove unreachable code
            }

            // Mark code after return/break/continue as unreachable
            match stmt {
                Statement::Return(_) | Statement::Break | Statement::Continue => {
                    let keep = reachable;
                    reachable = false;
                    keep
                }
                _ => true,
            }
        });
    }
}

impl Default for NativeOptimizer {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_constant_folding() {
        let optimizer = NativeOptimizer::new();

        // 2 + 3 should fold to 5
        let expr = Expr::BinaryOp {
            left: Box::new(Expr::Literal(Literal::Int(2))),
            op: BinaryOp::Add,
            right: Box::new(Expr::Literal(Literal::Int(3))),
        };

        let optimized = optimizer.optimize_expr(expr);
        assert_eq!(optimized, Expr::Literal(Literal::Int(5)));
    }

    #[test]
    fn test_algebraic_simplification() {
        let optimizer = NativeOptimizer::new();

        // x + 0 should simplify to x
        let expr = Expr::BinaryOp {
            left: Box::new(Expr::Identifier("x".to_string())),
            op: BinaryOp::Add,
            right: Box::new(Expr::Literal(Literal::Int(0))),
        };

        let optimized = optimizer.optimize_expr(expr);
        assert_eq!(optimized, Expr::Identifier("x".to_string()));
    }

    #[test]
    fn test_strength_reduction() {
        let optimizer = NativeOptimizer::new();

        // x * 8 should become x << 3
        let expr = Expr::BinaryOp {
            left: Box::new(Expr::Identifier("x".to_string())),
            op: BinaryOp::Mul,
            right: Box::new(Expr::Literal(Literal::Int(8))),
        };

        let optimized = optimizer.optimize_expr(expr);
        if let Expr::BinaryOp { op: BinaryOp::LShift, right, .. } = optimized {
            assert_eq!(*right, Expr::Literal(Literal::Int(3)));
        } else {
            panic!("Expected left shift");
        }
    }
}
