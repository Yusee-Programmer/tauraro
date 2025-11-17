//! Comprehensive test suite for the native C transpiler

use tauraro::ast::*;
use tauraro::codegen::c_transpiler::optimized_native::OptimizedNativeTranspiler;

#[test]
fn test_simple_integer_program() {
    let program = Program {
        statements: vec![
            Statement::VariableDef {
                name: "x".to_string(),
                type_annotation: Some(Type::Simple("int".to_string())),
                value: Some(Expr::Literal(Literal::Int(42))),
            },
            Statement::Expression(Expr::Call {
                func: Box::new(Expr::Identifier("print".to_string())),
                args: vec![Expr::Identifier("x".to_string())],
                kwargs: vec![],
            }),
        ],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("int64_t x = 42"));
    assert!(code.contains("printf"));
}

#[test]
fn test_arithmetic_operations() {
    let program = Program {
        statements: vec![
            Statement::VariableDef {
                name: "a".to_string(),
                type_annotation: Some(Type::Simple("int".to_string())),
                value: Some(Expr::Literal(Literal::Int(10))),
            },
            Statement::VariableDef {
                name: "b".to_string(),
                type_annotation: Some(Type::Simple("int".to_string())),
                value: Some(Expr::Literal(Literal::Int(20))),
            },
            Statement::VariableDef {
                name: "result".to_string(),
                type_annotation: None,
                value: Some(Expr::BinaryOp {
                    left: Box::new(Expr::Identifier("a".to_string())),
                    op: BinaryOp::Add,
                    right: Box::new(Expr::Identifier("b".to_string())),
                }),
            },
        ],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("int64_t a = 10"));
    assert!(code.contains("int64_t b = 20"));
    assert!(code.contains("(a + b)"));
}

#[test]
fn test_for_loop_with_range() {
    let program = Program {
        statements: vec![Statement::For {
            variable: "i".to_string(),
            variables: vec![],
            iterable: Expr::Call {
                func: Box::new(Expr::Identifier("range".to_string())),
                args: vec![Expr::Literal(Literal::Int(10))],
                kwargs: vec![],
            },
            body: vec![Statement::Expression(Expr::Call {
                func: Box::new(Expr::Identifier("print".to_string())),
                args: vec![Expr::Identifier("i".to_string())],
                kwargs: vec![],
            })],
            else_branch: None,
        }],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("for (int64_t i = 0; i < 10; i += 1)"));
    assert!(code.contains("printf"));
}

#[test]
fn test_if_statement() {
    let program = Program {
        statements: vec![
            Statement::VariableDef {
                name: "x".to_string(),
                type_annotation: Some(Type::Simple("int".to_string())),
                value: Some(Expr::Literal(Literal::Int(5))),
            },
            Statement::If {
                condition: Expr::Compare {
                    left: Box::new(Expr::Identifier("x".to_string())),
                    ops: vec![CompareOp::Gt],
                    comparators: vec![Expr::Literal(Literal::Int(0))],
                },
                then_branch: vec![Statement::Expression(Expr::Call {
                    func: Box::new(Expr::Identifier("print".to_string())),
                    args: vec![Expr::Literal(Literal::String("positive".to_string()))],
                    kwargs: vec![],
                })],
                elif_branches: vec![],
                else_branch: Some(vec![Statement::Expression(Expr::Call {
                    func: Box::new(Expr::Identifier("print".to_string())),
                    args: vec![Expr::Literal(Literal::String("negative".to_string()))],
                    kwargs: vec![],
                })]),
            },
        ],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("if ((x > 0))"));
    assert!(code.contains("} else {"));
}

#[test]
fn test_while_loop() {
    let program = Program {
        statements: vec![
            Statement::VariableDef {
                name: "count".to_string(),
                type_annotation: Some(Type::Simple("int".to_string())),
                value: Some(Expr::Literal(Literal::Int(0))),
            },
            Statement::While {
                condition: Expr::Compare {
                    left: Box::new(Expr::Identifier("count".to_string())),
                    ops: vec![CompareOp::Lt],
                    comparators: vec![Expr::Literal(Literal::Int(5))],
                },
                body: vec![Statement::SubscriptAssignment {
                    object: Expr::Identifier("count".to_string()),
                    index: Expr::Literal(Literal::Int(0)),
                    value: Expr::BinaryOp {
                        left: Box::new(Expr::Identifier("count".to_string())),
                        op: BinaryOp::Add,
                        right: Box::new(Expr::Literal(Literal::Int(1))),
                    },
                }],
                else_branch: None,
            },
        ],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("while ((count < 5))"));
}

#[test]
fn test_function_definition() {
    let program = Program {
        statements: vec![Statement::FunctionDef {
            name: "add".to_string(),
            params: vec![
                Param {
                    name: "a".to_string(),
                    type_annotation: Some(Type::Simple("int".to_string())),
                    default: None,
                    kind: ParamKind::Positional,
                },
                Param {
                    name: "b".to_string(),
                    type_annotation: Some(Type::Simple("int".to_string())),
                    default: None,
                    kind: ParamKind::Positional,
                },
            ],
            return_type: Some(Type::Simple("int".to_string())),
            body: vec![Statement::Return(Some(Expr::BinaryOp {
                left: Box::new(Expr::Identifier("a".to_string())),
                op: BinaryOp::Add,
                right: Box::new(Expr::Identifier("b".to_string())),
            }))],
            is_async: false,
            decorators: vec![],
            docstring: None,
        }],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("int64_t add(int64_t a, int64_t b)"));
    assert!(code.contains("return (a + b)"));
}

#[test]
fn test_type_conversions() {
    let program = Program {
        statements: vec![
            Statement::VariableDef {
                name: "x".to_string(),
                type_annotation: None,
                value: Some(Expr::Call {
                    func: Box::new(Expr::Identifier("int".to_string())),
                    args: vec![Expr::Literal(Literal::Float(3.14))],
                    kwargs: vec![],
                }),
            },
            Statement::VariableDef {
                name: "y".to_string(),
                type_annotation: None,
                value: Some(Expr::Call {
                    func: Box::new(Expr::Identifier("float".to_string())),
                    args: vec![Expr::Literal(Literal::Int(42))],
                    kwargs: vec![],
                }),
            },
        ],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("tauraro_int_from_float"));
    assert!(code.contains("tauraro_float_from_int"));
}

#[test]
fn test_class_definition() {
    let program = Program {
        statements: vec![Statement::ClassDef {
            name: "Point".to_string(),
            bases: vec![],
            body: vec![Statement::FunctionDef {
                name: "__init__".to_string(),
                params: vec![
                    Param {
                        name: "self".to_string(),
                        type_annotation: None,
                        default: None,
                        kind: ParamKind::Positional,
                    },
                    Param {
                        name: "x".to_string(),
                        type_annotation: Some(Type::Simple("int".to_string())),
                        default: None,
                        kind: ParamKind::Positional,
                    },
                    Param {
                        name: "y".to_string(),
                        type_annotation: Some(Type::Simple("int".to_string())),
                        default: None,
                        kind: ParamKind::Positional,
                    },
                ],
                return_type: None,
                body: vec![Statement::Pass],
                is_async: false,
                decorators: vec![],
                docstring: None,
            }],
            decorators: vec![],
            metaclass: None,
            docstring: None,
        }],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("struct Point_t"));
    assert!(code.contains("int ref_count"));
}

#[test]
fn test_builtin_functions() {
    let program = Program {
        statements: vec![
            Statement::VariableDef {
                name: "text".to_string(),
                type_annotation: Some(Type::Simple("str".to_string())),
                value: Some(Expr::Literal(Literal::String("hello".to_string()))),
            },
            Statement::VariableDef {
                name: "length".to_string(),
                type_annotation: None,
                value: Some(Expr::Call {
                    func: Box::new(Expr::Identifier("len".to_string())),
                    args: vec![Expr::Identifier("text".to_string())],
                    kwargs: vec![],
                }),
            },
            Statement::VariableDef {
                name: "absolute".to_string(),
                type_annotation: None,
                value: Some(Expr::Call {
                    func: Box::new(Expr::Identifier("abs".to_string())),
                    args: vec![Expr::Literal(Literal::Int(-5))],
                    kwargs: vec![],
                }),
            },
        ],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("strlen"));
    assert!(code.contains("tauraro_abs_int"));
}

#[test]
fn test_string_operations() {
    let program = Program {
        statements: vec![
            Statement::VariableDef {
                name: "name".to_string(),
                type_annotation: Some(Type::Simple("str".to_string())),
                value: Some(Expr::Literal(Literal::String("Tauraro".to_string()))),
            },
            Statement::Expression(Expr::Call {
                func: Box::new(Expr::Identifier("print".to_string())),
                args: vec![Expr::Identifier("name".to_string())],
                kwargs: vec![],
            }),
        ],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("char* name = \"Tauraro\""));
    assert!(code.contains("printf(\"%s\\n\", name)"));
}

#[test]
fn test_boolean_operations() {
    let program = Program {
        statements: vec![
            Statement::VariableDef {
                name: "flag".to_string(),
                type_annotation: Some(Type::Simple("bool".to_string())),
                value: Some(Expr::Literal(Literal::Bool(true))),
            },
            Statement::If {
                condition: Expr::Identifier("flag".to_string()),
                then_branch: vec![Statement::Expression(Expr::Call {
                    func: Box::new(Expr::Identifier("print".to_string())),
                    args: vec![Expr::Literal(Literal::String("flag is true".to_string()))],
                    kwargs: vec![],
                })],
                elif_branches: vec![],
                else_branch: None,
            },
        ],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("bool flag = true"));
    assert!(code.contains("if (flag)"));
}

#[test]
fn test_list_comprehension() {
    let program = Program {
        statements: vec![Statement::VariableDef {
            name: "squares".to_string(),
            type_annotation: None,
            value: Some(Expr::ListComp {
                element: Box::new(Expr::BinaryOp {
                    left: Box::new(Expr::Identifier("x".to_string())),
                    op: BinaryOp::Mul,
                    right: Box::new(Expr::Identifier("x".to_string())),
                }),
                generators: vec![Comprehension {
                    target: "x".to_string(),
                    iter: Expr::Call {
                        func: Box::new(Expr::Identifier("range".to_string())),
                        args: vec![Expr::Literal(Literal::Int(5))],
                        kwargs: vec![],
                    },
                    ifs: vec![],
                    is_async: false,
                }],
            }),
        }],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    // Should contain for loop
    assert!(code.contains("for (int64_t x"));
    // Should contain array allocation
    assert!(code.contains("realloc"));
}

#[test]
fn test_list_comprehension_with_filter() {
    let program = Program {
        statements: vec![Statement::VariableDef {
            name: "evens".to_string(),
            type_annotation: None,
            value: Some(Expr::ListComp {
                element: Box::new(Expr::Identifier("n".to_string())),
                generators: vec![Comprehension {
                    target: "n".to_string(),
                    iter: Expr::Call {
                        func: Box::new(Expr::Identifier("range".to_string())),
                        args: vec![Expr::Literal(Literal::Int(10))],
                        kwargs: vec![],
                    },
                    ifs: vec![Expr::Compare {
                        left: Box::new(Expr::BinaryOp {
                            left: Box::new(Expr::Identifier("n".to_string())),
                            op: BinaryOp::Mod,
                            right: Box::new(Expr::Literal(Literal::Int(2))),
                        }),
                        ops: vec![CompareOp::Eq],
                        comparators: vec![Expr::Literal(Literal::Int(0))],
                    }],
                    is_async: false,
                }],
            }),
        }],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    // Should contain for loop
    assert!(code.contains("for (int64_t n"));
    // Should contain if condition for filter
    assert!(code.contains("if"));
}

#[test]
fn test_nested_control_flow() {
    let program = Program {
        statements: vec![Statement::For {
            variable: "i".to_string(),
            variables: vec![],
            iterable: Expr::Call {
                func: Box::new(Expr::Identifier("range".to_string())),
                args: vec![Expr::Literal(Literal::Int(3))],
                kwargs: vec![],
            },
            body: vec![Statement::For {
                variable: "j".to_string(),
                variables: vec![],
                iterable: Expr::Call {
                    func: Box::new(Expr::Identifier("range".to_string())),
                    args: vec![Expr::Literal(Literal::Int(3))],
                    kwargs: vec![],
                },
                body: vec![Statement::Expression(Expr::Call {
                    func: Box::new(Expr::Identifier("print".to_string())),
                    args: vec![
                        Expr::Identifier("i".to_string()),
                        Expr::Identifier("j".to_string()),
                    ],
                    kwargs: vec![],
                })],
                else_branch: None,
            }],
            else_branch: None,
        }],
    };

    let mut transpiler = OptimizedNativeTranspiler::new();
    let result = transpiler.transpile_program(&program);

    assert!(result.is_ok());
    let code = result.unwrap();

    assert!(code.contains("for (int64_t i = 0; i < 3; i += 1)"));
    assert!(code.contains("for (int64_t j = 0; j < 3; j += 1)"));
}
