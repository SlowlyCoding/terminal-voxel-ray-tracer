BUILD_DIR := build

.PHONY: all clean

all: $(BUILD_DIR)
	@echo "Building project in $(BUILD_DIR)..."
	cd $(BUILD_DIR) && cmake .. && make

$(BUILD_DIR):
	@echo "Creating build directory: $(BUILD_DIR)"
	mkdir -p $(BUILD_DIR)

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) voxel_ray_tracer
