/etc/init.d/gpibenumsvc stop
/etc/init.d/nipal stop
rmmod gpibrtk nipalk nipal
/etc/init.d/nipal start
/etc/init.d/gpibenumsvc start
