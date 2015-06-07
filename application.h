#ifndef APPLICATION_H
#define APPLICATION_H

#define DEFAULT_MARGIN                                      10

#define DEFAULT_ABOUT_TEXT_WIDTH                            200

#define DEFAULT_MARGIN_TOOLBAR                              5

#define DIALOG_FIND_FIRST_COLUMN_WIDTH                      130
#define DIALOG_FIND_SECOND_COLUMN_WIDTH                     250
#define DIALOG_FIND_ROW_HEIGHT                              25

#define FIND_DIALOG_SAVE_SETTINGS_MAX                       25
#define SIDE_BAR_WIDTH                                      5

#define STRING_APPLICATION_NAME                             tr("Prj-Editor")
#define STRING_VERSION                                      tr("v0.1")
#define STRING_ORGANIZATION                                 tr("Timo Hannukkala")
#define STRING_LICENSE                                      tr("License: MIT license")
#define STRING_CREATOR                                      tr("Creator: ")

#define STRING_CLOSE                                        tr("Close")
#define STRING_ABOUT                                        tr("About")

#define STRING_ALL_FOLDERS                                  tr("All Folders")
#define STRING_PLAIN_TEXT                                   tr("Plain text")

#define STRING_TABULATOR_4_SPACES                           tr("Tab -> 4 Spaces")
#define STRING_TABULATOR_2_SPACES                           tr("Tab -> 2 Spaces")
#define STRING_TABULATOR_TABULATOR                          tr("Tab = Tab")

#define STRING_ADD                                          tr("Add")
#define STRING_ADD_NEW_COMMAND                              tr("Add new command")
#define STRING_CANCEL                                       tr("Cancel")
#define STRING_CHANGE_NAME                                  tr("Change name")
#define STRING_CHANGE_NAME_PROJECT_NAME                     tr("Change name project name")
#define STRING_FIND                                         tr("Find")
#define STRING_FIND_IN_FILES_FROM_THIS_PATH                 tr("Find in Files from this path")
#define STRING_CREATE                                       tr("Create")
#define STRING_FILENAME                                     tr("Filename")
#define STRING_FOLDER_NAME                                  tr("Folder name")
#define STRING_MOVE_DOWN                                    tr("Move down")
#define STRING_MOVE_UP                                      tr("Move up")
#define STRING_NEW_BASH_IN_BASH                             tr("New command in bash")
#define STRING_NEW_COMMAND                                  tr("New command")
#define STRING_ON_SUCCESS                                   tr("On success")
#define STRING_ON_FAILED                                    tr("On failed")
#define STRING_PROJECT                                      tr("Project")
#define STRING_REFRESH                                      tr("Refresh")
#define STRING_REMOVE_COMMAND                               tr("Remove command")
#define STRING_RENAME_FOLDER                                tr("Rename folder")
#define STRING_RENAME_FILE                                  tr("Rename file")
#define STRING_DELETE_FOLDER                                tr("Delete folder")
#define STRING_DELETE_FILE                                  tr("Delete file")
#define STRING_CONFIRM_DELETE                               tr("Confirm delete")
#define STRING_ARE_YOU_SURE_YOU_WANT_TO_DELETE_FILE         tr("Are you sure you want to delete the file: %1")
#define STRING_ARE_YOU_SURE_YOU_WANT_TO_DELETE_FOLDER       tr("Are you sure you want to delete the folder - and it's all subfolders and files: %1")
#define STRING_ERROR_RENAME_FOLDER                          tr("Renaming folder failed.")
#define STRING_ERROR_RENAME_FILE                            tr("Renaming file failed.")
#define STRING_REPLACE_ALL                                  tr("Replace all")
#define STRING_RUN_PROJECT                                  tr("Run project")
#define STRING_SAVE_SCRIPT                                  tr("Save script")
#define STRING_SCRIPT_NAME                                  tr("Script name")
#define STRING_SET_NEW_SCRIPT_NAME                          tr("Set new script name")
#define STRING_SORT_THE_LIST                                tr("Sort the list")

#define STRING_FOLDER_ALREADY_EXISTS                        tr("Folder already exists")
#define STRING_FILE_ALREADY_EXISTS                          tr("File already exists")

#define STRING_ERROR                                        tr("Error")
#define STRING_ERROR_CREATE_FOLDER_FAIL                     tr("Creating folder fail")
#define STRING_ERROR_CREATE_FILE_FAIL                       tr("Creating file fail")

#define STRING_OPEN_FOLDER                                  tr("Open folder")

#define STRING_ONE_OF_DOCUMENT_HAS_BEEN_MODIFIED            tr("At least one document has been modified.")
#define STRING_DOCUMENT_HAS_BEEN_MODIFIED                   tr("The document has been modified.")
#define STRING_DO_YOU_WANT_SAVE_YOUR_CHANGES                tr("Do you want to save your changes?")




#define STRING_FILE                                         tr("File")
#define STRING_LINE                                         tr("Line")
#define STRING_TEXT                                         tr("Text")


#define STRING_TOOLBAR_NEW                                  tr("New")
#define STRING_TOOLBAR_OPEN                                 tr("Open")
#define STRING_TOOLBAR_SAVE                                 tr("Save")
#define STRING_TOOLBAR_SAVE_ALL                             tr("Save All")

#define STRING_PROJECT_CLOSE                                tr("Close")
#define STRING_PROJECT_ADD_FOLDER                           tr("Add folder to list of Project Folders")
#define STRING_PROJECT_OPEN_FILE                            tr("Open file")
#define STRING_PROJECT_CREATE_FILE                          tr("Create new file")
#define STRING_PROJECT_CREATE_FOLDER                        tr("Create new folder")

#define STRING_UNTITLED                                     tr("Untitled")
#define STRING_RENAME                                       tr("Rename")

#define STRING_MENU_FILE                                    tr("&File")
#define STRING_MENU_FILE_NEW_FILE                           tr("&New file...")
#define STRING_MENU_FILE_OPEN_FILE                          tr("&Open file...")
#define STRING_MENU_FILE_SAVE_FILE                          tr("&Save file")
#define STRING_MENU_FILE_SAVE_AS                            tr("Save file As")
#define STRING_MENU_FILE_SAVE_ALL                           tr("Save all")

#define STRING_MENU_EDIT                                    tr("&Edit")

#define STRING_MENU_UNDO                                    tr("Undo")
#define STRING_MENU_REDO                                    tr("Redo")

#define STRING_MENU_CUT                                     tr("Cut")
#define STRING_MENU_COPY                                    tr("Copy")
#define STRING_MENU_PASTE                                   tr("Paste")


#define STRING_MENU_SEARCH                                  tr("Search")

#define STRING_MENU_FIND                                    tr("Find")
#define STRING_MENU_FIND_NEXT                               tr("Find Next")
#define STRING_MENU_FIND_PREVIOUS                           tr("Find Previous")
#define STRING_MENU_FIND_IN_FILES                           tr("Find in Files")
#define STRING_MENU_REPLACE                                 tr("Replace")

#define STRING_MENU_HELP                                    tr("Help")
#define STRING_MENU_ABOUT                                   tr("About %1")

#define STRING_FIND_REPLACE                                 tr("Replace:")
#define STRING_FIND_FROM                                    tr("Find from:")
#define STRING_FIND_FILE_TYPES                              tr("File types:")

#define STRING_FIND_MATCH_CASE                              tr("Match case")
#define STRING_FIND_INCLUDE_SUB_FOLDERS                     tr("Include Subfolders")
#define STRING_FIND_INCLUDE_HIDDEN                          tr("Include Hidden")

#define STRING_FIND_SEARCH                                  tr("Search")
#define STRING_FIND_CURRENT_DOCUMENT                        tr("Current document")
#define STRING_FIND_ALL_OPEN_DOCUMENTS                      tr("All open documents")
#define STRING_FIND_IN_SELECTED_TEXT                        tr("In selected text")

#define SHORTCUT_KEY_NEW                                    tr("Ctrl+N")
#define SHORTCUT_KEY_OPEN                                   tr("Ctrl+O")

#define SHORTCUT_KEY_SAVE                                   tr("Ctrl+S")
#define SHORTCUT_KEY_SAVE_ALL                               tr("Ctrl+Shift+S")

#define SHORTCUT_KEY_UNDO                                   tr("Ctrl+Z")
#define SHORTCUT_KEY_REDO                                   tr("Ctrl+Y")

#define SHORTCUT_KEY_CUT                                    tr("Ctrl+X")
#define SHORTCUT_KEY_COPY                                   tr("Ctrl+C")
#define SHORTCUT_KEY_PASTE                                  tr("Ctrl+V")

#define SHORTCUT_KEY_FIND_NEXT                              tr("F3")
#define SHORTCUT_KEY_FIND_PREVIOUS                          tr("Shift+F3")
#define SHORTCUT_KEY_FIND                                   tr("Ctrl+F")
#define SHORTCUT_KEY_FIND_IN_FILES                          tr("Ctrl+Shift+F")
#define SHORTCUT_KEY_REPLACE                                tr("Ctrl+H")

#define SHORTCUT_KEY_RUN_PROJECT                            tr("F5")

#endif // APPLICATION_H
