.PHONY: clean All

All:
	@echo "----------Building project:[ GDecimal - Release ]----------"
	@cd "GDecimal" && "$(MAKE)" -f  "GDecimal.mk"
clean:
	@echo "----------Cleaning project:[ GDecimal - Release ]----------"
	@cd "GDecimal" && "$(MAKE)" -f  "GDecimal.mk" clean
