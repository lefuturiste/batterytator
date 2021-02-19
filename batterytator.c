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
#define SLEEP_DURATION 15

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char* batID;

char* readFile(char* path) {
  FILE *fp;

  fp = fopen(path, "r");
  char buff[255];
  char *content = buff;
  fgets(buff, 255, (FILE*)fp);
  fclose(fp);

  return content;
}

char * getPath(char* pathToAppend) {
  static char newStr[255] = "/sys/class/power_supply/";
  strcat(newStr, batID);
  strcat(newStr, pathToAppend);
  return newStr;
}

void sendNotif(char* urgency, char* message) {
  char newStr[255] = {};
  strcat(newStr, "notify-send ");
  strcat(newStr, "--urgency=");
  strcat(newStr, urgency);
  strcat(newStr, " --app-name='");
  strcat(newStr, APP_NAME);
  strcat(newStr, "' '");
  strcat(newStr, message);
  strcat(newStr, "'");
  //system("notify-send  --urgency=critical  --app-name='Hello world' 'Hello'");

  //printf("%s \n", newStr);
  system(newStr);
}

void playSound(char* path) {
  char newStr[255] = {};
  strcat(newStr, "mplayer ");
  strcat(newStr, path);
  strcat(newStr, " > /dev/null 2>&1");

  //printf("%s \n", newStr);
  system(newStr);
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    batID = DEFAULT_BAT_ID;
  } else {
    batID = argv[1];
  }

  printf("Service batterytator started...\n");
  
  pid_t pid = getpid();
  printf("PID: %lu \n", pid);

  char path[255];
  strcpy(path, getPath("/capacity"));
  printf("Capacity file path: %s \n", path);
  
  int warnLevels[] = {
    75,
    50,
    30,
    20,
    15,
    10,
    6
  };
  char* warnLevelsMessages[] = {
    "just to inform you",
    "less than half way",
    "you may want to think about your charger",
    "this start to be serious",
    "please charge your battery",
    "take action NOW!!",
    "!!!!!!!!"
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