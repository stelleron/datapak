# GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  datapak_config = debug
  test_config = debug
  comptest_config = debug
endif
ifeq ($(config),release)
  datapak_config = release
  test_config = release
  comptest_config = release
endif

PROJECTS := datapak test comptest

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

datapak:
ifneq (,$(datapak_config))
	@echo "==== Building datapak ($(datapak_config)) ===="
	@${MAKE} --no-print-directory -C . -f datapak.make config=$(datapak_config)
endif

test: datapak
ifneq (,$(test_config))
	@echo "==== Building test ($(test_config)) ===="
	@${MAKE} --no-print-directory -C . -f test.make config=$(test_config)
endif

comptest:
ifneq (,$(comptest_config))
	@echo "==== Building comptest ($(comptest_config)) ===="
	@${MAKE} --no-print-directory -C . -f comptest.make config=$(comptest_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f datapak.make clean
	@${MAKE} --no-print-directory -C . -f test.make clean
	@${MAKE} --no-print-directory -C . -f comptest.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   datapak"
	@echo "   test"
	@echo "   comptest"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"