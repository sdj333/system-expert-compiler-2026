import os
import lit.formats

config.name = "SCCP Tests"
config.test_format = lit.formats.ShTest()
config.suffixes = [".ll"]
config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.dirname(__file__)
