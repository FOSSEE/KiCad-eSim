#!python.exe
# EASY-INSTALL-ENTRY-SCRIPT: 'setuptools==36.0.1.post20170616','console_scripts','easy_install'
__requires__ = 'setuptools==36.0.1.post20170616'
import re
import sys
from pkg_resources import load_entry_point

if __name__ == '__main__':
    sys.argv[0] = re.sub(r'(-script\.pyw?|\.exe)?$', '', sys.argv[0])
    sys.exit(
        load_entry_point('setuptools==36.0.1.post20170616', 'console_scripts', 'easy_install')()
    )
