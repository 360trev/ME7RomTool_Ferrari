.PHONY: clean All

All:
	@echo "----------Building project:[ me7romtool - Release ]----------"
	@"$(MAKE)" -f  "me7romtool.mk"
clean:
	@echo "----------Cleaning project:[ me7romtool - Release ]----------"
	@"$(MAKE)" -f  "me7romtool.mk" clean
