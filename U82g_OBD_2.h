struct pid_name{
  const char *name;
  int16_t PID_Number;
  unsigned int numValues;
};
struct pid_name PID_List[] = {
  // pid NAME                     PID_Number, BYTES
  {"PIDs supported",               0x00,          4 }, //    0
  {"status DTCs",                  0x01,          4 }, //    1
  {"Freeze DTC",                   0x02,          2 }, //    2
  {"Fuel system status",           0x03,          2 }, //    3
  {"ENG LOAD",                     0x04,          1 }, //    4
  {"COOLANT",                      0x05,          1 }, //    5
  {"Short fuel trim Bk1",          0x06,          1 }, //    6
  {"Long fuel trim Bk 1",          0x07,          1 }, //    7
  {"Short fuel trim Bk2",          0x08,          1 }, //    8
  {"Long fuel trim Bk 2",          0x09,          1 }, //    9
  {"Fuel PRESSURE",                0x0A,          1 }, //    10
  {"BOOST",                        0x0B,          1 }, //    11
  {"RPM",                          0x0C,          2 }, //    12
  {"SPEED",                        0x0D,          1 }, //    13
  {"TIM ADV",                      0x0E,          1 }, //    14
  {"ITA",                          0x0F,          1 }, //    15
  {"MAF",                          0x10,          2 }, //    16
  {"THR POS",                      0x11,          1 }, //    17
  {"secondary air status",         0x12,          1 }, //    18
  {"O2 sensors present",           0x13,          1 }, //    19
  {"O2 Sensor 1",                  0x14,          2 }, //    20
  {"O2 Sensor 2",                  0x15,          2 }, //    21
  {"O2 Sensor 3",                  0x16,          2 }, //    22
  {"O2 Sensor 4",                  0x17,          2 }, //    23
  {"O2 Sensor 5",                  0x18,          2 }, //    24
  {"O2 Sensor 6",                  0x19,          2 }, //    25
  {"O2 Sensor 7",                  0x1A,          2 }, //    26
  {"O2 Sensor 8",                  0x1B,          2 }, //    27
  {"OBD standards",                0x1C,          1 }, //    28
  {"O2 sensors present",           0x1D,          1 }, //    29
  {"Aux input status",             0x1E,          1 }, //     30
  {"Engine Runtime",               0x1F,          2 }, //     31
  {"PIDs(21-40)",                  0x20,          4 }, //     32
  {"DIST (MIL)on",                 0x21,          2 }, //     33
  {"Fuel Rail Pres",               0x22,          2 }, //     34
  {"Fuel Gauge Pressure",          0x23,          2 }, //     35
  {"O2 Sensor 1",                  0x24,          4 }, //     36
  {"O2 Sensor 2",                  0x25,          4 }, //     37
  {"O2 Sensor 3",                  0x26,          4 }, //     38
  {"O2 Sensor 4",                  0x27,          4 }, //     39
  {"O2 Sensor 5",                  0x28,          4 }, //     40
  {"O2 Sensor 6",                  0x29,          4 }, //     41
  {"O2 Sensor 7",                  0x2A,          4 }, //     42
  {"O2 Sensor 8",                  0x2B,          4 }, //     43
  {"Commanded EGR",                0x2C,          1 }, //     44
  {"EGR Error",                    0x2D,          1 }, //     45
  {"commanded Evap purge",         0x2E,          1 }, //     46
  {"Fuel Tank Level",              0x2F,          1 }, //     47
  {"Warm-ups",                     0x30,          1 }, //     48
  {"Distance traveled",            0x31,          2 }, //     49
  {"Eva Pressure",                 0x32,          2 }, //     50
  {"Baro Pressure",                0x33,          1 }, //     51
  {"O2Sensor 1",                   0x34,          4 }, //     52 
  {"O2Sensor 2",                   0x35,          4 }, //     53
  {"O2Sensor 3",                   0x36,          4 }, //     54
  {"O2Sensor 4",                   0x37,          4 }, //     55
  {"O2Sensor 5",                   0x38,          4 }, //     56
  {"O2Sensor 6",                   0x39,          4 }, //     57
  {"O2Sensor 7",                   0x3A,          4 }, //     58
  {"O2Sensor 8",                   0x3B,          4 }, //     59
  {"CatTemp Bk1 Sen 1",            0x3C,          2 }, //     60
  {"CatTemp Bk2 Sen 1",            0x3D,          2 }, //     61 
  {"CatTemp Bk1 Sen 2",            0x3E,          2 }, //     62
  {"CatTemp Bk2 Sen 2",            0x3F,          2 }, //     63
  {"PIDs(41-60)",                  0x40,          4 }, //     64
  {"Monitor drive cycle",          0x41,          4 }, //     65
  {"module voltage",               0x42,          2 }, //     66
  {"Absolute load value",          0x43,          2 }, //     67
  {"commanded F A ratio",          0x44,          2 }, //     68
  {"Relative thr position",        0x45,          1 }, //     69
  {"Ambient air temp",             0x46,          1 }, //     70
  {"Abs thr position B",           0x47,          1 }, //     71
  {"Abs thr position C",           0x48,          1 }, //     72
  {"Abs thr position D",           0x49,          1 }, //     73
  {"Abs thr position E",           0x4A,          1 }, //     74
  {"Abs thr position F",           0x4B,          1 }, //     75
  {"Commanded thr actuator",       0x4C,          1 }, //     76
  {"Time run with MIL on",         0x4D,          2 }, //     77
  {"Time codes cleared",           0x4E,          2 }, //     78
  {"Max value",                    0x4F,          4 }, //     79
  {"Max value airflowrate",        0x50,          4 }, //     80
  {"Fuel Type",                    0x51,          1 }, //     81
  {"Ethanol fuel %",               0x52,          1 }, //     82
  {"Abs Evap Press",               0x53,          2 }, //     83
  {"Evap  pressure",               0x54,          2 }, //     84
  {"Short O2  trim bk1 bk3",       0x55,          2 }, //     85
  {"Long  O2  trim bk1 bk3",       0x56,          2 }, //     86
  {"Short O2  trim bk2 bk4",       0x57,          2 }, //     87
  {"Long O2  trim bk2 bk4" ,       0x58,          2 }, //     88
  {"Fuel rail abs pressure",       0x59,          2 }, //     89
  {"Relative  pedal position",     0x5A,          1 }, //     90
  {"Hybrid battery remaining",     0x5B,          1 }, //     91
  {"Engine oil temperature",       0x5C,          1 }, //     92
  {"Injection timing",             0x5D,          2 }, //     93
  {"Engine fuel rate",             0x5E,          2 }, //     94
  {"Emission "       ,             0x5F,          1 }, //     95
  {"PIDs [61 - 80]"  ,             0x60,          4 }, //     96
  {"Driver's demand  torque",      0x61,          1 }, //     97
  {"Actual torque",                0x62,          1 }, //     98
  {"Reference torque",             0x63,          2 }, //     99
  {"Engine percent torque",        0x64,          5 }, //     100
  {"Aux in/out supported",         0x65,          2 }, //     101
  {"Mass air flow sensor",         0x66,          5 }, //     102
  {"Coolant temperature",          0x67,          3 }, //     103   
  {"Intake air temp",              0x68,          7 }, //     104
  {"Commanded EGR/EGR Err",        0x69,          7 }, //     105
  {"Diesel intake air flow",       0x6A,          5 }, //     106
  {"Ex gas recirc temp",           0x6B,          5 }, //     107
  {"Commanded thr actuator",       0x6C,          5 }, //     108
  {"Fuel pre control",             0x6D,          6 }, //     109
  {"Injection pressure control",   0x6E,          5 }, //     110
  {"BOOST",                        0x6F,          3 }, //     111
  {"BOOST",                        0x70,          9 }, //     112
  {"(VGT) control",                0x71,          5 }, //     113
  {"Wastegate control",            0x72,          5 }, //     114
  {"Exhaust pressure",             0x73,          5 }, //     115
  {"Turbo RPM",                    0x74,          5 }, //     116
  {"Turbo Temp",                   0x75,          7 }, //     117
  {"Turbo Temp",                   0x76,          7 }, //     118
  {"ITA",                          0x77,          5 }, //     119
  {"(EGT) 1",                      0x78,          9 }, //     120
  {"(EGT) 2",                      0x79,          9 }, //     121
  {"(DPF)",                        0x7A,          7 }, //     122
  {"(DPF)",                        0x7B,          7 }, //     123
  {"(DPF) temperature",            0x7C,          9 }, //     124
  {"NOx NTE",                      0x7D,          1 }, //     125
  {"PM NTE",                       0x7E,          1 }, //     126
  {"Engine run time",              0x7F,          13 }, //     127
  {"PIDs[81 - A0]",                0x80,          4 }, //     128
  {"Engine run time",              0x81,          21 }, //     129
  {"Engine run time",              0x82,          21 }, //     130
  {"NOx sensor",                   0x83,          5 }, //     131
  {"Manifold surface temp",        0x84,          1 }, //     132
  {"NOx reagent system",           0x85,          10 }, //     133
  {"(PM) sensor",                  0x86,          5 }, //     134
  {"Intake manifold abs pres",     0x87,          5 }, //     135
  {"SCR Induce System",            0x88,          13 }, //     136
  {"Run Time for AECD",            0x89,          41 }, //     137
  {"Run Time for AECD",            0x8A,          41 }, //     138
  {"Diesel Aftertreatment ",       0x8B,          7 }, //     139
  {"O2 Sensor (Wide Range)",       0x8C,          16 }, //     140
  {"Throttle Position G",          0x8D,          1 }, //     141 
  {"Engine Friction ",             0x8E,          1 }, //     142
  {"PM Sensor Bank 1 & 2",         0x8F,          5 }, //     143
};
