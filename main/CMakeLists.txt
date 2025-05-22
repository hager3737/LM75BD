idf_component_register(SRCS "lm75db.c" "main.c"
                    PRIV_REQUIRES spi_flash 
                    INCLUDE_DIRS "."
                    REQUIRES driver
                    )
