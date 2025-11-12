use std::env;
use std::process::Command;
use std::path::Path;

fn main() {
    // Check if webviewtk feature is enabled using environment variable
    let features = env::var("CARGO_FEATURE_WEBVIEWTK").is_ok();

    if !features {
        // Feature not enabled, nothing to do
        return;
    }

    println!("cargo:warning=WebViewTK feature enabled - checking platform dependencies...");

    // Detect platform and install dependencies
    let target_os = env::var("CARGO_CFG_TARGET_OS").unwrap_or_default();

    match target_os.as_str() {
        "windows" => install_webview2_windows(),
        "linux" => install_webkitgtk_linux(),
        "macos" => {
            println!("cargo:warning=macOS detected - WebKit is built-in, no installation needed");
        }
        _ => {
            println!("cargo:warning=Unknown platform: {}. You may need to install webview dependencies manually.", target_os);
        }
    }
}

fn install_webview2_windows() {
    println!("cargo:warning=Windows detected - checking for WebView2 Runtime...");

    // Check if WebView2 is already installed by looking for the installation directory
    let install_paths = [
        r"C:\Program Files (x86)\Microsoft\EdgeWebView\Application",
        r"C:\Program Files\Microsoft\EdgeWebView\Application",
    ];

    for path in &install_paths {
        if Path::new(path).exists() {
            println!("cargo:warning=WebView2 Runtime is already installed at {}!", path);
            return;
        }
    }

    // Also check registry as fallback
    let check_installed = Command::new("reg")
        .args(&["query", "HKLM\\SOFTWARE\\WOW6432Node\\Microsoft\\EdgeUpdate\\Clients\\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}", "/v", "pv"])
        .output();

    if let Ok(output) = check_installed {
        if output.status.success() {
            println!("cargo:warning=WebView2 Runtime is already installed (detected via registry)!");
            return;
        }
    }

    println!("cargo:warning=WebView2 Runtime not found. Attempting to download and install...");

    // Download WebView2 Runtime bootstrapper
    let url = "https://go.microsoft.com/fwlink/p/?LinkId=2124703";
    let installer_path = env::temp_dir().join("MicrosoftEdgeWebview2Setup.exe");

    println!("cargo:warning=Downloading WebView2 Runtime from Microsoft...");

    // Use PowerShell to download the installer (more reliable than curl on Windows)
    let download_result = Command::new("powershell")
        .args(&[
            "-Command",
            &format!(
                "Invoke-WebRequest -Uri '{}' -OutFile '{}'",
                url,
                installer_path.display()
            )
        ])
        .status();

    match download_result {
        Ok(status) if status.success() => {
            println!("cargo:warning=Download complete. Installing WebView2 Runtime...");

            // Run the installer silently
            let install_result = Command::new(&installer_path)
                .args(&["/silent", "/install"])
                .status();

            match install_result {
                Ok(install_status) if install_status.success() => {
                    println!("cargo:warning=WebView2 Runtime installed successfully!");

                    // Clean up installer
                    let _ = std::fs::remove_file(&installer_path);
                }
                Ok(install_status) => {
                    println!("cargo:warning=WebView2 installer exited with code: {:?}", install_status.code());
                    println!("cargo:warning=Installation may have failed. You may need to install manually from:");
                    println!("cargo:warning=https://developer.microsoft.com/microsoft-edge/webview2/");
                }
                Err(e) => {
                    println!("cargo:warning=Failed to run WebView2 installer: {}", e);
                    println!("cargo:warning=Please install WebView2 Runtime manually from:");
                    println!("cargo:warning=https://developer.microsoft.com/microsoft-edge/webview2/");
                }
            }
        }
        Ok(status) => {
            println!("cargo:warning=Failed to download WebView2 Runtime (exit code: {:?})", status.code());
            println!("cargo:warning=Please install manually from:");
            println!("cargo:warning=https://developer.microsoft.com/microsoft-edge/webview2/");
        }
        Err(e) => {
            println!("cargo:warning=Failed to download WebView2 Runtime: {}", e);
            println!("cargo:warning=Please install manually from:");
            println!("cargo:warning=https://developer.microsoft.com/microsoft-edge/webview2/");
        }
    }
}

fn install_webkitgtk_linux() {
    println!("cargo:warning=Linux detected - checking for WebKitGTK...");

    // Check if webkit2gtk is installed using pkg-config
    let check_webkit = Command::new("pkg-config")
        .args(&["--exists", "webkit2gtk-4.0"])
        .status();

    if let Ok(status) = check_webkit {
        if status.success() {
            println!("cargo:warning=WebKitGTK is already installed!");
            return;
        }
    }

    println!("cargo:warning=WebKitGTK not found. Attempting to install...");

    // Try to detect the Linux distribution
    if Path::new("/etc/debian_version").exists() {
        // Debian/Ubuntu
        println!("cargo:warning=Detected Debian/Ubuntu-based system");
        println!("cargo:warning=Installing libwebkit2gtk-4.0-dev...");

        let install_result = Command::new("sudo")
            .args(&["apt-get", "install", "-y", "libwebkit2gtk-4.0-dev"])
            .status();

        match install_result {
            Ok(status) if status.success() => {
                println!("cargo:warning=WebKitGTK installed successfully!");
            }
            _ => {
                println!("cargo:warning=Automatic installation failed. Please run manually:");
                println!("cargo:warning=sudo apt-get install libwebkit2gtk-4.0-dev");
            }
        }
    } else if Path::new("/etc/fedora-release").exists() {
        // Fedora
        println!("cargo:warning=Detected Fedora-based system");
        println!("cargo:warning=Installing webkit2gtk3-devel...");

        let install_result = Command::new("sudo")
            .args(&["dnf", "install", "-y", "webkit2gtk3-devel"])
            .status();

        match install_result {
            Ok(status) if status.success() => {
                println!("cargo:warning=WebKitGTK installed successfully!");
            }
            _ => {
                println!("cargo:warning=Automatic installation failed. Please run manually:");
                println!("cargo:warning=sudo dnf install webkit2gtk3-devel");
            }
        }
    } else if Path::new("/etc/arch-release").exists() {
        // Arch Linux
        println!("cargo:warning=Detected Arch-based system");
        println!("cargo:warning=Installing webkit2gtk...");

        let install_result = Command::new("sudo")
            .args(&["pacman", "-S", "--noconfirm", "webkit2gtk"])
            .status();

        match install_result {
            Ok(status) if status.success() => {
                println!("cargo:warning=WebKitGTK installed successfully!");
            }
            _ => {
                println!("cargo:warning=Automatic installation failed. Please run manually:");
                println!("cargo:warning=sudo pacman -S webkit2gtk");
            }
        }
    } else {
        println!("cargo:warning=Unknown Linux distribution");
        println!("cargo:warning=Please install WebKitGTK manually for your distribution:");
        println!("cargo:warning=- Debian/Ubuntu: sudo apt-get install libwebkit2gtk-4.0-dev");
        println!("cargo:warning=- Fedora: sudo dnf install webkit2gtk3-devel");
        println!("cargo:warning=- Arch: sudo pacman -S webkit2gtk");
    }
}
