from PyQt5.QtWidgets import QMessageBox


_version = "0.0.0"
_author = "solort"
_github_link = "<a href='https://github.com/XiaoyuXiao1998/UnrealTaichiIntegration'>Source link</a>"
_about_text = "Version: {}<br>Author: {}<br>Github: {}".format(_version,
                                                               _author,
                                                               _github_link
                                                               )


def show(parent):
    _about = QMessageBox.about(
        parent,
        "About",
        _about_text
    )
    return _about
