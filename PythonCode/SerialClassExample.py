serialComm = serialLink()
while(1):
    if False:
        break
    else:
        serialComm.write(10)
        time.sleep(1)
        serialComm.read(1)
        time.sleep(1)
