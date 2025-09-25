use std::env;

fn main() {
    let current_dir = env::current_dir().unwrap();
    let c_file = current_dir.join("advanced_lib.c");
    
    if !c_file.exists() {
        eprintln!("C file not found: {:?}", c_file);
        std::process::exit(1);
    }
    
    // Set OUT_DIR for cc crate
    env::set_var("OUT_DIR", &current_dir);
    
    // Use cc crate to compile
    let mut build = cc::Build::new();
    build
        .file(&c_file)
        .shared_flag(true)
        .out_dir(&current_dir);
    
    // Try to compile and handle the output manually
    match build.try_compile("advanced_lib") {
        Ok(_) => println!("Successfully compiled C library"),
        Err(e) => {
            eprintln!("Failed to compile: {}", e);
            
            // Try alternative approach - generate the command and run it manually
            println!("Attempting manual compilation...");
            
            // Try with different compiler commands
            let commands = vec![
                ("gcc", vec!["-shared", "-fPIC", "-o", "advanced_lib.dll", "advanced_lib.c", "-lm"]),
                ("clang", vec!["-shared", "-fPIC", "-o", "advanced_lib.dll", "advanced_lib.c", "-lm"]),
                ("cl", vec!["/LD", "advanced_lib.c", "/Fe:advanced_lib.dll"]),
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