import qbs
//import qbs.FileInfo
//import qbs.ModUtils

Product {
    name: "hih9000.elf"
    type: "application"

    Depends {name:"cpp"}

    consoleApplication: true
    cpp.positionIndependentCode: false
    cpp.commonCompilerFlags: [
        "-mcpu=cortex-m4",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=hard",
        "-mthumb",
        "-Wall",
        "-ffunction-sections",
        "-g",
        "-O0",
        "--specs=nano.specs",
        //"-Wno-unused-parameter",
    ]
    cpp.linkerFlags: [
        "-mcpu=cortex-m4",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=hard",
        "-mthumb",
        "-nostartfiles",
        "-Wall",
        "-g",
        "-O0",
        "-Wl,--gc-sections",
        "-lm",
        "-lgcc",
        "-lc",
        //"-lnosys",
    ]
    cpp.defines: [
        "STM32F407VG",
        "STM32F4XX",
        "STM32F40_41xxx",
        "USE_STDPERIPH_DRIVER",
        "__ASSEMBLY__",
        "__FPU_USED",
    ]
    Properties {
            condition: cpp.debugInformation
            cpp.defines: outer.concat("DEBUG")
    }
    cpp.cxxFlags: [ "-std=c++11"]
    cpp.cFlags: [ "-std=c11" ]
    cpp.warningLevel: "all"

    //project files:


    cpp.linkerScripts: [
        "arm-gcc-link.ld"
    ]

    cpp.includePaths: [
        "main",
        "premain",
        "cmsis",
        "spl/inc",
        "pressure"


    ]

    Group {
        name: "libs"
        files: [
            //cmsis
            "cmsis/*.h",
            "cmsis/*.c",
            "cmsis/*.s",
            "premain/*.h",
            "premain/*.c",

            //stm SPL
            "spl/inc/*.h",
            "spl/src/stm32f4xx_rcc.c",
            "spl/src/stm32f4xx_gpio.c",
            "spl/src/stm32f4xx_syscfg.c",
            "spl/src/stm32f4xx_tim.c",
            "spl/src/stm32f4xx_dma.c",
            "spl/src/stm32f4xx_i2c.c",
            "spl/src/misc.c",

        ]
        cpp.cxxFlags: [ "-std=c++11" ]
        cpp.cFlags: [
            "-std=c11",
            "-Wno-unused-parameter",
            "-Wno-sign-compare",
            "-Wno-empty-body",
        ]
    }

    files: [
        "main/*.h",
        "main/*.c",
        "pressure/*.h",
        "pressure/*.c",


        //link script
        "*.ld",
    ]
}

