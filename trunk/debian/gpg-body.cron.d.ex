#
# Regular cron jobs for the gpg-body package
#
0 4	* * *	root	[ -x /usr/bin/gpg-body_maintenance ] && /usr/bin/gpg-body_maintenance
