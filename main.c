#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

void time_converter(long long timestamp)
{
    int days_since_epoch = timestamp / 86400; // curr time/seconds in a day

    // Define arrays for month names and days per month
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    const int days_per_month[] = {31, 28, 31, 30, 31, 30,
                                  31, 31, 30, 31, 30, 31};

    // Calculate year, month, and day
    int year = 1970;
    int month = 0;
    int day = 1;
    while (days_since_epoch > 365 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
        days_since_epoch -= 365 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        year++;
    }
    while (days_since_epoch >= days_per_month[month]) {
        days_since_epoch -= days_per_month[month];
        month++;
    }
    day += days_since_epoch;

    // Calculate time (hour and minute)
    int hour = (timestamp % 86400) / 3600;
    int minute = (timestamp % 3600) / 60;

    // Print the formatted date and time
    printf("%s %02d %02d:%02d\n", months[month], day, hour, minute);

}


int main() 
{
// Open the current directory
DIR *dir = opendir(".");
if (dir == NULL) {
    perror("opendir");
    return 1; // Exit with error status
}

// Read directory entries
struct dirent *entry;
long total_blocks = 0;
while ((entry = readdir(dir)) != NULL)
{
    struct stat file_stat;
    // Get metadata of the file
    if (stat(entry->d_name, &file_stat) == -1) {
        perror("stat");
        return 1;
    }
    total_blocks += file_stat.st_size;
    // Print file name and metadata
    printf("Mode: %ld, Num links: %ld, UID: %ld, Size: %ld, Time: %ld File: %s ", file_stat.st_mode, file_stat.st_nlink, file_stat.st_uid, file_stat.st_size, file_stat.st_mtime, entry->d_name);
    time_converter(file_stat.st_mtime);
}
printf("There should be blocks but it is bytes %ld\n", total_blocks);

// Close the directory stream
closedir(dir);
}