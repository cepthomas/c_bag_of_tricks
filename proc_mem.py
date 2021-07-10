
# Collect and process output from instrumented unit test build.


import sys

if len(sys.argv) != 2:
    print('>>>>>>>>>>>>> Bad args')
    exit()

# Collect and analyze input lines.

allocs = dict()

print('Open %s' % sys.argv[1])

f = open(sys.argv[1], "r")

# 0.085361,DBG,MEM,64,---,0104c248,64,"C:\Dev\repos\c_bag_of_tricks\source\private\list.c"
# 0       ,1  ,2  ,3 ,4  ,5       ,6 ,7

# 0.000000,DBG,MEM,123,+++,012372f8,123,"C:\Dev\repos\c_bag_of_tricks\source\private\list.c"


for line in f.readlines():
    # print("%s" % line)
    # Split the line and see if it is one we are interested in.

    parts = line.split(',')
    # print(parts)

    if len(parts) >= 8 and parts[2] == 'MEM':
        address = parts[5]
        line = parts[6]
        file = parts[7].strip()
        fileline = "%s(%s)" % (file, line)

        if parts[4] == '+++':
            if address in allocs:
                print("%s: Duplicate alloc at %s" % (fileline, address))
            else:
                allocs.setdefault(address, fileline)
        elif parts[4] == '---':
            if address not in allocs:
                print("%s: Freeing invalid pointer at %s" % (fileline, address))
            else:
                allocs.pop(address)
        else:
            # Ignore
            pass


for address, fileline in allocs.items():
    print("%s: Unfreed memory at %s" % (fileline, address))
