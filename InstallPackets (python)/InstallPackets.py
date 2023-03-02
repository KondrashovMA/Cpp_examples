#!/usr/bin/python3

import os
import subprocess

class InstallCommand(object):  # используется для хранения и обработки комманд установки пакетов
    successInstallingFlag = False  # флаг успеха установки
    _packetCommand = ""  # имя пакета для установки
    _packetLogFileName = ""  # имя файла для логирования

    def __init__(self, command):
        self._packetCommand = command
        self._packetLogFileName = str(command).replace(" ", "-")

    def setPacketCommand(self, command):
        self._packetCommand = command

    def getPacketCommand(self):
        return self._packetCommand

    # очистка логов для вывода
    def clearOutLog(self):
        if os.path.exists("Logs/out/" + self._packetLogFileName + ".out.txt"):
            clearLogCommand = "> Logs/out/" + self._packetLogFileName + ".out.txt"
            os.system(clearLogCommand)

    # очистка логов для ошибок
    def clearErrLog(self):
        if os.path.exists("Logs/err/" + self._packetLogFileName + ".err.txt"):
            clearLogCommand = "> Logs/err/" + self._packetLogFileName + ".err.txt"
            os.system(clearLogCommand)

    # создаёт список параметров для subprocess.Popen
    def createCommandParameters(self):
        commandList = ["sudo", "apt-get", "install"]
        commandList.extend(str(self._packetCommand).split(" "))
        commandList.append("-y")
        return commandList

    def writeInformationToLog(self, path, filename, logType, data):
        if not os.path.exists(path):
            os.mkdir(path)
        with open(path + filename + logType + ".txt", "w") as fileErr:
            fileErr.write(data)
        fileErr.close()

    def installPacket(self):
        self.clearOutLog()
        self.clearErrLog()

        if not os.path.exists("Logs/"):
            os.mkdir("Logs")

        try:
            # создаём процесс с выполнением списка параметров, получаемых из имени устанавливаемого пакета
            executeCommandProcess = subprocess.Popen(self.createCommandParameters(),
                                                     text=True,
                                                     stdout=subprocess.PIPE,
                                                     stderr=subprocess.STDOUT)

            executeCommandProcess.wait(timeout = 180)  # ожидаем завершение процеса, иначе сразу несколько займут apt-get
            stdout, stderr = executeCommandProcess.communicate()


            self.writeInformationToLog(path = "Logs/out/", filename = self._packetLogFileName, logType = "out", data = stdout)

            if stderr is not None:
                self.writeInformationToLog(path="Logs/err/", filename=self._packetLogFileName, logType="err", data = stderr)

            self.successInstallingFlag = True
            return True

        except subprocess.CalledProcessError as e: # ошибка выполнения процесса
            self.writeInformationToLog(path="Logs/err/", filename=self._packetLogFileName, logType="err", data=str(e))

        except subprocess.TimeoutExpired as e: #ошибка по времени
            self.writeInformationToLog(path="Logs/err/", filename=self._packetLogFileName, logType="err", data=str(e))

        return False


class FoldersHandler:  # класс для создания и перемещения нужных файлов по папкам на плате
    def __int__(self):
        pass

# тк там много шагов, то проще ставить просто через os.system с выводом в консоль
def installCmake():
    listOfCommand = ["sudo apt purge --auto-remove cmake -y",
                     "sudo apt install software-properties-common lsb-release",
                     "sudo apt clean all",
                     "wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null",
                     'sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main',
                     "sudo apt update",
                     "sudo apt install -y kitware-archive-keyring",
                     "sudo rm /etc/apt/trusted.gpg.d/kitware.gpg",
                     "sudo apt install cmake -y"]
    for command in listOfCommand:
        print("command: " + command + " ")
        os.system(command)

# проверяем наличие cmake и версию, если она меньше искомой либо cmake вообще нет, то возвращаем True
def checkCmake() -> bool:
    try:
        cmakeCheckCommand = subprocess.Popen(["cmake", "--version"],
                                             text=True,
                                             stdout=subprocess.PIPE)

        cmakeCheckCommand.wait()  # ожидаем завершение процеса, тк исполнение одного процесса блокирует apt-get
        stdout = cmakeCheckCommand.communicate()
        version = str(stdout).split(" ")[2][:4]
        if float(version) < 3.16:
            return True
        else:
            print("Cmake version is " + version, " it's ok")
    except FileNotFoundError as e:
        print("Cmake don't install, start installing: ")
        return True
    return False

if __name__ == '__main__':
    commandList = []  # список комманд
    installPSQL = InstallCommand("postgresql postgresql-contrib")
    installChrome = InstallCommand("chromium-browser")
    installJava = InstallCommand("openjdk-8-jdk")
    installSupervisor = InstallCommand("supervisor")
    installGit = InstallCommand("git")
    installMake = InstallCommand("make")
    installGPlusPlus = InstallCommand("g++")
    installNinjaBuild = InstallCommand("ninja-build")
    installTShark = InstallCommand("tshark")

    commandList.extend([installPSQL, installChrome, installJava, installSupervisor,
                        installGit, installMake, installGPlusPlus, installNinjaBuild, installTShark])

    unsuccessfulCommands = []  # команды, которые не прошли

    for command in commandList:
        print("Start installing of " + command.getPacketCommand() + ":")
        resultInstalled = command.installPacket()
        if resultInstalled:
            print("Packet " + command.getPacketCommand() + " has been installed")
            print()
        else:
            unsuccessfulCommands.append(command)

    # делаем вторую попытку поставить пакеты, которые не удалось поставить (например из-за сбоя интернета)
    for command in unsuccessfulCommands:
        command.installPacket()

    print("install cmake:")
    if checkCmake():
        installCmake()
        checkCmake()