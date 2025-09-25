use std::process::Command;

fn main() {
    // Try to use cc crate if available, otherwise use system compiler
    let output = Command::new("cargo")
        .args(&["run", "--bin", "build_c_lib"])
        .output();
        
    match output {
        Ok(result) => {
            if result.status.success() {
                println!("Successfully built C library");
            } else {
                println!("Failed to build C library: {}", String::from_utf8_lossy(&result.stderr));
            }
        }
        Err(e) => {
            println!("Error running build command: {}", e);
        }
    }
}