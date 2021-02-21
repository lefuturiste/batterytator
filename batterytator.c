/**
 * Batterytator
 * A simple battery monitor script that will check the capacity every 15 seconds
 * 
 * Created by: lefuturiste
 * Repository: https://github.com/lefuturiste/batterytator
 * License: MIT
 * */
#define DEFAULT_BAT_ID "BAT1"
#define APP_NAME "Battery"
#define SLEEP_DURATION 20

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

char* batID;
char* user;
char tmpMsg[400];
char logPath[255] = {};

/**
 * Get current date and time
 */
char* getDate() {
  FILE *fp;
  char buff[255];
  char *content = buff;
  fp = popen("date '+%d/%m/%Y %H:%M:%S'", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  while (fgets(buff, 20, fp) != NULL) {
    return content;
  }
}

/**
 * Get output of the 'whoami' unix command
 */
char* getUser() {
  FILE *fp;
  char buff[255];
  char *content = buff;
  fp = popen("whoami", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  while (fgets(buff, sizeof(buff), fp) != NULL) {
    buff[strcspn(buff, "\n")] = 0;
    return content;
  }
}

/**
 * Log a message to Stdout and to a log file on the disk
 */
void logMessage() {
  char message[500] = {};
  strcat(message, getDate());
  strcat(message, ": ");
  strcat(message, tmpMsg);

  printf(message);

  // clear the tempMsg string
  memset(tmpMsg, 0, sizeof(tmpMsg));

  FILE *fp;

  fp = fopen(logPath, "a+"); // open the file in append mode

  if(fp != NULL) {
    fputs(message, fp);
    fclose(fp);
  } 
}

/**
 * Read the first 255 bytes of a file
 */
char* readFile(char* path) {
  FILE *fp;

  fp = fopen(path, "r");
  char buff[255];
  char *content = buff;
  fgets(buff, 255, (FILE*)fp);
  fclose(fp);

  return content;
}

char* getPath(char* pathToAppend) {
  static char newPath[255] = "/sys/class/power_supply/";
  //static char newPath[255] = "/home/mbess/batterystat/";
  strcat(newPath, batID);
  strcat(newPath, pathToAppend);

  return newPath;
}

void runCmd(char* cmd) {
  char newStr[350] = {};

  if (user == "root") {
    strcat(newStr, "su ");
    strcat(newStr, user);
    strcat(newStr, " -c \"");
    strcat(newStr, cmd);
    strcat(newStr, "\"");
  } else {
    strcat(newStr, cmd);
  }

  sprintf(tmpMsg, "%s \n", newStr);
  logMessage();

  system(newStr);
}

/**
 * Send notification using 'notify-send' command
 */
void sendNotif(char* urgency, char* message) {
  char newStr[350] = {};
  strcat(newStr, "export DISPLAY=:0 && notify-send ");
  strcat(newStr, "--urgency=");
  strcat(newStr, urgency);
  strcat(newStr, " --app-name='");
  strcat(newStr, APP_NAME);
  strcat(newStr, "' '");
  strcat(newStr, message);
  strcat(newStr, "'");
  //system("notify-send  --urgency=critical  --app-name='Hello world' 'Hello'");

  runCmd(newStr);
}

/**
 * Play a music file using the `mplayer` command
 */
void playSound(char* path) {
  char newStr[255] = {};
  strcat(newStr, "mplayer ");
  strcat(newStr, path);
  strcat(newStr, " > /dev/null 2>&1");

  runCmd(newStr);
}

// Usage ./batterytator <USER> <BAT_ID>
int main(int argc, char* argv[]) {
  user = "mbess";
  batID = DEFAULT_BAT_ID;
  if (argc > 1) {
    user = argv[1];
  }
  if (argc > 2) {
    batID = argv[2];
  }
  
  // generate log path
  strcat(logPath, "/home/");
  strcat(logPath, user);
  strcat(logPath, "/batterytator.log");

  // delete log file
  char deleteCmd[255] = {};
  sprintf(deleteCmd, "rm %s", logPath);
  system(deleteCmd);
  
  strcat(tmpMsg, "Service batterytator started...\n");
  logMessage();

  //system("whoami");

  sprintf(tmpMsg, "WhoAmI: %s \n", getUser());
  logMessage();

  pid_t pid = getpid();
  
  sprintf(tmpMsg, "PID: %lu \n", pid);
  logMessage();

  char path[255];
  strcpy(path, getPath("/capacity"));
  sprintf(tmpMsg, "Capacity file path: %s \n", path);
  logMessage();

  sprintf(tmpMsg, "User as configured: %s \n", user);
  logMessage();
  
  int warnLevels[] = {
    75,
    50,
    30,
    20,
    15,
    10,
    5
  };
  char* warnLevelsMessages[] = {
    "just to inform you",
    "less than half way",
    "you may want to think about your charger",
    "this start to be serious",
    "please charge your battery",
    "take action NOW",
    "too late..."
  };
  int levelsCount = (int) (sizeof(warnLevels)/sizeof(int));
  int state[levelsCount];
  int currentIndex = -1;
  int capacity;
  int lastCapacity = -1;
  while (1) {
    capacity = atoi(readFile(path));
    if (lastCapacity != capacity) {
      //printf("%d \n", capacity);

      int warn = -1;
      for (int i = 0; i < levelsCount; i++) {
        if (capacity <= warnLevels[i]) {
          if (state[i] == 1) {
            continue;
          }
          state[i] = 1;
          warn = i;
        } else {
          state[i] = 0;
        }
      }
      lastCapacity = capacity;

      if (warn != -1) {
        char newMsg[255];
        sprintf(newMsg, "%d", capacity);
        strcat(newMsg, "%, ");
        strcat(newMsg, warnLevelsMessages[warn]);
        sendNotif("critical", newMsg);
        playSound("/usr/share/sounds/freedesktop/stereo/bell.oga");
      }
    }
    sleep(SLEEP_DURATION);
  }
}