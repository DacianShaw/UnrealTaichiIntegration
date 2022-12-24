from PyQt5.QtWidgets import QAction
from PyQt5.QtGui import QIcon


class Bookmark:
    def __init__(self, master):
        self.FILE = {
            'new': QAction(QIcon("assets/New.png"), "&New File", master),
            'sep1': self.__get_separator(master),
            'save': QAction(QIcon("assets/Save.png"), "&Save", master),
            'save_as': QAction(QIcon("assets/SaveAs.png"), "Save &As...", master),
            'sep2': self.__get_separator(master),
            'open_file': QAction(QIcon("assets/Open.png"), "&Open File...", master),
            'sep3': self.__get_separator(master),
            'close_tab': QAction("Close window", master),
            'sep4': self.__get_separator(master),
            'exit': QAction("&Exit", master),
        }
        self.EDIT = {
            'undo': QAction(QIcon("assets/Undo.png"), "&Undo", master),
            'redo': QAction("&Redo", master),
            'compile': QAction(QIcon("assets/Compile.png"), "&Compile", master),
            'sep1': self.__get_separator(master),
            'cut': QAction("Cu&t", master),
            'copy': QAction("&Copy", master),
            'paste': QAction("&Paste", master),
            'sep2': self.__get_separator(master),
            'replace_in_file': QAction("&Replace in file", master),
        }
        self.VIEW = {
            'status_bar': QAction("Show s&tatus bar", master),
        }
        self.PREFERENCES = {
            'soon': QAction("&Soon", master),
        }
        self.HELP = {
            'about': QAction("&About", master),
        }

    def __get_separator(self, master):
        separator = QAction(master)
        separator.setSeparator(True)
        return separator
