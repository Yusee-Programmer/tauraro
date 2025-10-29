# File Dialog Demo for Tauraro GUI
# Demonstrates open and save file dialogs

import gui

def main():
    # Show an information message first
    gui.show_info("File Dialog Demo", "This demo will show file dialogs.")
    
    # Show open file dialog
    filename = gui.open_file_dialog(
        title="Select a file to open",
        filter="Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0"
    )
    
    if filename:
        gui.show_info("File Selected", f"You selected: {filename}")
    else:
        gui.show_warning("No File Selected", "You didn't select a file.")
    
    # Show save file dialog
    filename = gui.save_file_dialog(
        title="Save file as",
        filter="Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0",
        default_ext="txt"
    )
    
    if filename:
        gui.show_info("File to Save", f"You want to save as: {filename}")
    else:
        gui.show_warning("No File Selected", "You didn't specify a file to save.")
    
    print("File dialog demo completed.")

if __name__ == "__main__":
    main()