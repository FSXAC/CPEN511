printf '\n' | ./receiver 200 > receiver_out.txt &
printf 'abcdefghijklmnopqrstuvwxyz0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZ_lcall#5\n' | ./sender
cat receiver_out.txt
