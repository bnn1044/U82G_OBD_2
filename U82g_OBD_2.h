
struct pid_name{
  const char *name;
  int16_t PID_Number;
};

/*struct pid_name FavouritePID_List[] = {
  {"COOLANT",PID_COOLANT_TEMP},   //1
  {" BOOST ",PID_BOOST_CONTROL},    //2
  {"TIME ADV",PID_TIMING_ADVANCE},                //3
  {"ITA  ",PID_CHARGE_AIR_TEMP,},//4
};*/
struct pid_name PID_List[] = {
  {"PIDs supported",               0x00}, //    0
  {"status DTCs",                  0x01}, //    1
  {"Freeze DTC",                   0x02}, //    2
  {"Fuel system status",           0x03}, //    3
  {"Calculated engine load",       0x04}, //    4
  {"coolant ",                     0x05}, //    5
  {"Short fuel trim Bk1",          0x06}, //    6
  {"Long fuel trim Bk 1",          0x07}, //    7
  {"Short fuel trim Bk2",          0x08}, //    8
  {"Long fuel trim Bk 2",          0x09}, //    9
  {"Fuel pressure",                0x0A}, //    10
  {"Intake man pressure",          0x0B}, //    11
  {"RPM",                          0x0C}, //    12
  {"Vehicle speed",                0x0D}, //    13
  {"Timing advance",               0x0E}, //    14
  {"Intake air temperature",       0x0F}, //    15
  {"MAF air flow rate",            0x10}, //    16
  {"Throttle position",            0x11}, //    17
  {"secondary air status",         0x12}, //   18
  {"O2 sensors present",           0x13}, //    19
  {"O2 Sensor 1",                  0x14}, //    20
  {"O2 Sensor 2",                  0x15}, //    21
  {"O2 Sensor 3",                  0x16}, //    22
  {"O2 Sensor 4",                  0x17}, //    23
  {"O2 Sensor 5",                  0x18}, //    24
  {"O2 Sensor 6",                  0x19}, //    25
  {"O2 Sensor 7",                  0x1A}, //     26
  {"O2 Sensor 8",                  0x1B}, //     27
  {"OBD standards",                0x1C}, //     28
  {"O2 sensors present",           0x1D}, //     29
  {"Aux input status",             0x1E}, //     30
  {"Engine Runtime",               0x1F}, //     31
  {"PIDs(21-40)",                  0x20}, //     32
  {"DIST (MIL)on",                 0x21}, //     33
  {"Fuel Rail Pres",               0x22}, //     34
  {"Fuel Gauge Pressure",          0x23}, //     35
  {"O2 Sensor 1",                  0x24}, //     36
  {"O2 Sensor 2",                  0x25}, //     37
  {"O2 Sensor 3",                  0x26}, //     38
  {"O2 Sensor 4",                  0x27}, //     39
  {"O2 Sensor 5",                  0x28}, //     40
  {"O2 Sensor 6",                  0x29}, //     41
  {"O2 Sensor 7",                  0x2A}, //     42
  {"O2 Sensor 8",                  0x2B}, //     43
  {"Commanded EGR",                0x2C}, //     44
  {"EGR Error",                    0x2D}, //     45
  {"commanded Evap purge",         0x2E}, //     46
  {"Fuel Tank Level",              0x2F}, //     47
  {"Warm-ups",                     0x30}, //     48
  {"Distance traveled",            0x31}, //     49
  {"Eva Pressure",                 0x32}, //     50
  {"Baro Pressure",                0x33}, //     51
  {"O2Sensor 1",                   0x34}, //     52 
  {"O2Sensor 2",                   0x35}, //     53
  {"O2Sensor 3",                   0x36}, //     54
  {"O2Sensor 4",                   0x37}, //     55
  {"O2Sensor 5",                   0x38}, //     56
  {"O2Sensor 6",                   0x39}, //     57
  {"O2Sensor 7",                   0x3A}, //     58
  {"O2Sensor 8",                   0x3B}, //     59
  {"CatTemp Bk1 Sen 1",            0x3C}, //     60
  {"CatTemp Bk2 Sen 1",            0x3D}, //     61 
  {"CatTemp Bk1 Sen 2",            0x3E}, //     62
  {"CatTemp Bk2 Sen 2",            0x3F}, //     63
  {"PIDs(41-60)",                  0x40}, //     64
  {"Monitor drive cycle",          0x41}, //     65
  {"module voltage",               0x42}, //     66
  {"Absolute load value",          0x43}, //     67
  {"commanded F A ratio",          0x44}, //     68
  {"Relative thr position",        0x45}, //     69
  {"Ambient air temp",             0x46}, //     70
  {"Abs thr position B",           0x47}, //     71
  {"Abs thr position C",           0x48}, //     72
  {"Abs thr position D",           0x49}, //     73
  {"Abs thr position E",           0x4A}, //     74
  {"Abs thr position F",           0x4B}, //     75
  {"Commanded thr actuator",       0x4C}, //     76
  {"Time run with MIL on",         0x4D}, //     77
  {"Time codes cleared",           0x4E}, //     78
  {"Max value",                    0x4F}, //     79
  {"Max value airflowrate",        0x50}, //     80
  {"Fuel Type",                    0x51}, //     81
  {"Ethanol fuel %",               0x52}, //     82
  {"Abs Evap Press",               0x53}, //     83
  {"Evap  pressure",               0x54}, //     84
  {"Short O2  trim bk1 bk3",       0x55}, //     85
  {"Long  O2  trim bk1 bk3",       0x56}, //     86
  {"Short O2  trim bk2 bk4",       0x57}, //     87
  {"Long O2  trim bk2 bk4" ,       0x58}, //     88
  {"Fuel rail abs pressure",       0x59}, //     89
  {"Relative  pedal position",     0x5A}, //     90
  {"Hybrid battery remaining",     0x5B}, //     91
  {"Engine oil temperature",       0x5C}, //     92
  {"Injection timing",             0x5D}, //     93
  {"Engine fuel rate",             0x5E}, //     94
  {"Emission "       ,             0x5F}, //     95
  {"PIDs [61 - 80]"  ,             0x60}, //     96
  {"Driver's demand  torque",      0x61}, //     97
  {"Actual torque",                0x62}, //     98
  {"Reference torque",             0x63}, //     99
  {"Engine percent torque",        0x64}, //     100
  {"Aux in/out supported",         0x65}, //     101
  {"Mass air flow sensor",         0x66}, //     102
  {"Coolant temperature",          0x67}, //     103   
  {"Intake air temp",              0x68}, //     104
  {"Commanded EGR/EGR Err",        0x69}, //     105
  {"Diesel intake air flow",       0x6A}, //     106
  {"Ex gas recirc temp",           0x6B}, //     107
  {"Commanded thr actuator",       0x6C}, //     108
  {"Fuel pressure control",        0x6D}, //     109
  {"Injection pressure control",   0x6E}, //     110
  {"Turbocharger inlet pres",      0x6F}, //     111
  {" Boost ",                      0x70}, //     112
  {"(VGT) control",                0x71}, //     113
  {"Wastegate control",            0x72}, //     114
  {"Exhaust pressure",             0x73}, //     115
  {"Turbo RPM",                    0x74}, //     116
  {"Turbo Temp",                   0x75}, //     117
  {"Turbo Temp",                   0x76}, //     118
  {" ITA ",                        0x77}, //     119
  {"(EGT) Bank 1",                 0x78}, //     120
  {"(EGT) Bank2",                  0x79}, //     121
  {"(DPF)",                        0x7A}, //     122
  {"(DPF)",                        0x7B}, //     123
  {"(DPF) temperature",            0x7C}, //     124
  {"NOx NTE",                      0x7D}, //     125
  {"PM NTE",                       0x7E}, //     126
  {"Engine run time",              0x7F}, //     127
  {"PIDs[81 - A0]",                0x80}, //     128
  {"Engine run time",              0x81}, //     129
  {"Engine run time",              0x82}, //     130
  {"NOx sensor",                   0x83}, //     131
  {"Manifold surface temp",        0x84}, //     132
  {"NOx reagent system",           0x85}, //     133
  {"(PM) sensor",                  0x86}, //     134
  {"Intake manifold abs pres",     0x87}, //     135
  {"SCR Induce System",            0x88}, //     136
  {"Run Time for AECD",            0x89}, //     137
  {"Run Time for AECD",            0x8A}, //     138
  {"Diesel Aftertreatment ",       0x8B}, //     139
  {"O2 Sensor (Wide Range)",       0x8C}, //     140
  {"Throttle Position G",          0x8D}, //     141 
  {"Engine Friction ",             0x8E}, //     142
  {"PM Sensor Bank 1 & 2",         0x8F}, //     143
};
