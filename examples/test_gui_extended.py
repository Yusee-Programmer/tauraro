# Test Extended GUI Features
# Simple test to verify new GUI functionality

import gui

def test_constants():
    # Test that new constants are available
    assert hasattr(gui, 'BS_PUSHBUTTON'), "BS_PUSHBUTTON constant missing"
    assert hasattr(gui, 'ES_LEFT'), "ES_LEFT constant missing"
    assert hasattr(gui, 'SWP_NOSIZE'), "SWP_NOSIZE constant missing"
    assert hasattr(gui, 'ICC_STANDARD_CLASSES'), "ICC_STANDARD_CLASSES constant missing"
    print("✓ All new constants available")

def test_functions():
    # Test that new functions are available
    assert hasattr(gui, 'create_button'), "create_button function missing"
    assert hasattr(gui, 'create_textbox'), "create_textbox function missing"
    assert hasattr(gui, 'set_control_text'), "set_control_text function missing"
    assert hasattr(gui, 'get_control_text'), "get_control_text function missing"
    assert hasattr(gui, 'set_window_position'), "set_window_position function missing"
    assert hasattr(gui, 'get_window_rect'), "get_window_rect function missing"
    assert hasattr(gui, 'client_to_screen'), "client_to_screen function missing"
    assert hasattr(gui, 'init_common_controls'), "init_common_controls function missing"
    assert hasattr(gui, 'message_loop'), "message_loop function missing"
    assert hasattr(gui, 'send_message'), "send_message function missing"
    print("✓ All new functions available")

def test_structures():
    # Test that new structures are available
    assert hasattr(gui, 'RECT'), "RECT class missing"
    assert hasattr(gui, 'MSG'), "MSG class missing"
    assert hasattr(gui, 'INITCOMMONCONTROLSEX'), "INITCOMMONCONTROLSEX class missing"
    print("✓ All new structures available")

def main():
    print("Testing Extended GUI Features...")
    test_constants()
    test_functions()
    test_structures()
    print("All tests passed! Extended GUI library is ready.")

if __name__ == "__main__":
    main()