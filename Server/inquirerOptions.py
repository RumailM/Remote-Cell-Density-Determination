import inquirer

MAIN_MENU_CMDS = ["Monitor", "Automatic Test", "Settings", "Quit" ]
YN = ["Yes", "No"]

mainMenu = [
  inquirer.List(name = 'MainMenu',
                message="Select a Mode:",
                choices= MAIN_MENU_CMDS
            ),
]

quitMenu = [
  inquirer.List(name = 'Quit',
                message="Are you sure you wish to quit?:",
                choices= YN
            ),
]
