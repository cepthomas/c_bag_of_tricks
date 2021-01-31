

# Collect and process output from instrumented unit test build.


import sys

# Collect and analyze input lines.

allocs = dict()

for line in sys.stdin:
    # sys.stdout.write("%s" % line)

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
                sys.stdout.write("%s: Duplicate alloc.\n" % fileline)
            else:
                allocs.setdefault(address, fileline)
        elif parts[0] == '---':
            if address not in allocs:
                sys.stdout.write("%s: Freeing invalid pointer %s.\n" % (fileline, address))
            else:
                allocs.pop(address)
        else:
            # Ignore
            pass


for address, fileline in allocs.items():
    sys.stdout.write("%s: Unfreed memory %s.\n" % (fileline, address))
