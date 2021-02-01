
# Collect and process output from instrumented unit test build.


import sys

if len(sys.argv) != 2:
    print('>>>>>>>>>>>>> Bad args')
    exit()

# Collect and analyze input lines.

allocs = dict()

print('Open %s' % sys.argv[1])

f = open(sys.argv[1], "r")

for line in f.readlines():
    # print("%s" % line)

    # Split the line and see if it is one we are interested in.
    # +++,008485E0,396,"C:\Dev\repos\c-bag-of-tricks\cbot_module\private\stringx.c"

    parts = line.split(',')

    if len(parts) == 4:
        address = parts[1]
        line = parts[2]
        file = parts[3].strip()
        fileline = "%s(%s)" % (file, line)

        if parts[0] == '+++':
            if address in allocs:
                print("%s: Duplicate alloc at %s" % (fileline, address))
            else:
                allocs.setdefault(address, fileline)
        elif parts[0] == '---':
            if address not in allocs:
                print("%s: Freeing invalid pointer at %s" % (fileline, address))
            else:
                allocs.pop(address)
        else:
            # Ignore
            pass


for address, fileline in allocs.items():
    print("%s: Unfreed memory at %s" % (fileline, address))
