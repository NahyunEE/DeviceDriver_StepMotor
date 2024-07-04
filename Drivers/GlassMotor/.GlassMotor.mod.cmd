cmd_/home/nahyun/Drivers/GlassMotor/GlassMotor.mod := printf '%s\n'   GlassMotor/GlassMotor.o | awk '!x[$$0]++ { print("/home/nahyun/Drivers/"$$0) }' > /home/nahyun/Drivers/GlassMotor/GlassMotor.mod
