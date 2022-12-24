import os
import sys
import unreal


def main():
    menus = unreal.ToolMenus.get()
    main_menu = menus.find_menu("LevelEditor.MainMenu")
    if not main_menu:
        print("Failed to find the 'Main' menu. Something is wrong in the force!")

    entry = unreal.ToolMenuEntry(
        name="Python.Tools",
        type=unreal.MultiBlockType.MENU_ENTRY,
        insert_position=unreal.ToolMenuInsert("", unreal.ToolMenuInsertType.FIRST)
    )
    entry.set_label("Taichi Editor")

    cmd_type = unreal.ToolMenuStringCommandType.COMMAND
    cmd = "py " + sys.argv[0].split("setup.py")[0] + "TaichiEditor/taichi_editor.py"
    entry.set_string_command(cmd_type, "", cmd) 

    # entry.set_string_command(unreal.ToolMenuStringCommandType.PYTHON, "Name", "print('这是一个测试')")
    script_menu = main_menu.add_sub_menu(main_menu.get_name(), "TaichiTools", "TaichiTools", "TaichiTools")
    script_menu.add_menu_entry("Scripts", entry)
    menus.refresh_all_widgets()


if __name__ == '__main__':
    main()