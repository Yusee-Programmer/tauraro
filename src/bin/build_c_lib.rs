use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        eprintln!("Usage: {} <c_file>", args[0]);
        std::process::exit(1);
    }
    
    let current_dir = env::current_dir().unwrap();
    let c_file = current_dir.join(&args[1]);
    
    if !c_file.exists() {
        eprintln!("C file not found: {:?}", c_file);
        std::process::exit(1);
    }
    
    // Set OUT_DIR for cc crate
    env::set_var("OUT_DIR", &current_dir);
    
    // Use cc crate to compile
    let mut build = cc::Build::new();
    let output_name = c_file.file_stem().unwrap().to_str().unwrap();
    build
        .file(&c_file)
        .out_dir(&current_dir);
    
    // Try to compile and handle the output manually
    match build.try_compile(output_name) {
        Ok(_) => println!("Successfully compiled C library"),
        Err(e) => {
            eprintln!("Failed to compile: {}", e);
            
            // Try alternative approach - generate the command and run it manually
            println!("Attempting manual compilation...");
            
            // Try with different compiler commands
            let exe_name = format!("{}.exe", output_name);
            let cl_output_arg = format!("/Fe:{}", exe_name);
            let commands = vec![
                ("gcc", vec!["-o", &exe_name, &args[1], "-lm"]),
                ("clang", vec!["-o", &exe_name, &args[1], "-lm"]),
                ("cl", vec![&args[1], &cl_output_arg]),
            ];
            
            for (compiler, args) in commands {
                println!("Trying {} with args: {:?}", compiler, args);
                match std::process::Command::new(compiler)
                    .args(&args)
                    .current_dir(&current_dir)
                    .output() {
                    Ok(output) => {
                        if output.status.success() {
                            println!("Successfully compiled with {}", compiler);
                            return;
                        } else {
                            println!("{} failed: {}", compiler, String::from_utf8_lossy(&output.stderr));
                        }
                    }
                    Err(e) => {
                        println!("{} not available: {}", compiler, e);
                    }
                }
            }
            
            eprintln!("All compilation attempts failed. Please install a C compiler (gcc, clang, or MSVC).");
            std::process::exit(1);
        }
    }
}