#!/system/usr/bin/sh

echo "Release wake lock"
echo test > /sys/power/wake_unlock
