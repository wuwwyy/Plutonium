
.PHONY: lib clean

lib-pc:
	@$(MAKE) -C Plutonium/ pc

lib-switch:
	@$(MAKE) -C Plutonium/ switch

clean:
	@$(MAKE) clean -C Plutonium/
	@$(MAKE) clean -C Example/

pc: lib-pc
	@$(MAKE) -C Example/ pc

switch: lib-switch
	@$(MAKE) -C Example/ switch