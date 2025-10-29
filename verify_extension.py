#!/usr/bin/env python3
"""
Verification script for Tauraro VSCode extension
This script checks if the extension files are properly configured
"""

import json
import os

def verify_package_json():
    """Verify package.json configuration"""
    package_path = "vscode-tauraro/package.json"
    
    if not os.path.exists(package_path):
        print("❌ package.json not found")
        return False
    
    with open(package_path, 'r') as f:
        package = json.load(f)
    
    # Check if all extensions are supported
    languages = package.get('contributes', {}).get('languages', [])
    if not languages:
        print("❌ No languages configured")
        return False
    
    extensions = languages[0].get('extensions', [])
    required_extensions = ['.tr', '.tau', '.tauraro']
    
    for ext in required_extensions:
        if ext not in extensions:
            print(f"❌ Extension {ext} not supported")
            return False
    
    # Check for icon configuration
    icon = languages[0].get('icon')
    if not icon:
        print("⚠️  Icon not configured (optional)")
    else:
        light_icon = icon.get('light')
        dark_icon = icon.get('dark')
        if light_icon and dark_icon:
            print("✅ Icon configuration found")
            if os.path.exists(f"vscode-tauraro/{light_icon}"):
                print("✅ Icon file exists")
            else:
                print(f"❌ Icon file {light_icon} not found")
        else:
            print("⚠️  Incomplete icon configuration")
    
    print("✅ package.json verification passed")
    print(f"   Supported extensions: {extensions}")
    return True

def verify_tm_language():
    """Verify tmLanguage.json syntax"""
    tm_path = "vscode-tauraro/syntaxes/tauraro.tmLanguage.json"
    
    if not os.path.exists(tm_path):
        print("❌ tmLanguage.json not found")
        return False
    
    try:
        with open(tm_path, 'r') as f:
            # Try to parse JSON
            data = json.load(f)
        
        # Check if scopeName is properly set
        if data.get('scopeName') != 'source.tauraro':
            print("❌ scopeName is not properly set")
            return False
            
        print("✅ tmLanguage.json syntax validation passed")
        return True
    except json.JSONDecodeError as e:
        print(f"❌ tmLanguage.json syntax error: {e}")
        return False

def verify_sample_files():
    """Verify sample files exist"""
    sample_files = [
        "examples/tauraro_sample.tr",
        "examples/tauraro_sample.tau",
        "examples/tauraro_sample.tauraro"
    ]
    
    for file_path in sample_files:
        if not os.path.exists(file_path):
            print(f"❌ Sample file {file_path} not found")
            return False
    
    print("✅ All sample files exist")
    return True

def verify_language_config():
    """Verify language-configuration.json"""
    config_path = "vscode-tauraro/language-configuration.json"
    
    if not os.path.exists(config_path):
        print("❌ language-configuration.json not found")
        return False
    
    try:
        with open(config_path, 'r') as f:
            json.load(f)
        print("✅ language-configuration.json validation passed")
        return True
    except json.JSONDecodeError as e:
        print(f"❌ language-configuration.json syntax error: {e}")
        return False

def verify_icon_file():
    """Verify icon file exists"""
    icon_path = "vscode-tauraro/tauraro-img.jpg"
    
    if os.path.exists(icon_path):
        print("✅ Icon file exists")
        return True
    else:
        print("❌ Icon file not found")
        return False

def main():
    print("Tauraro VSCode Extension Verification")
    print("=" * 40)
    
    checks = [
        verify_package_json,
        verify_tm_language,
        verify_language_config,
        verify_icon_file,
        verify_sample_files
    ]
    
    all_passed = True
    for check in checks:
        if not check():
            all_passed = False
    
    print("\n" + "=" * 40)
    if all_passed:
        print("🎉 All verification checks passed!")
        print("The Tauraro VSCode extension is properly configured.")
        print("\nSupported file extensions:")
        print("  - .tr (Tauraro source files)")
        print("  - .tau (Tauraro source files)")
        print("  - .tauraro (Tauraro source files)")
        print("\nFeatures:")
        print("  - Syntax highlighting")
        print("  - Language icon")
        print("  - Multi-language keywords (English/Hausa)")
    else:
        print("❌ Some verification checks failed.")
        print("Please review the errors above.")

if __name__ == "__main__":
    main()