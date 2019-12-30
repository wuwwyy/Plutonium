
.PHONY: lib clean

lib:
	@$(MAKE) -C Plutonium/

clean:
	@$(MAKE) clean -C Plutonium/
	@$(MAKE) clean -C Example/

switch: lib
	@$(MAKE) -C Example/