

#define REG_COMMENTARY "\"[^)\"]*[\\] [^)\"]*\"|(((^| )|\n)[\\] [^\n$]*)"
#define REG_COMMENTARY_MULTI "(\"[^)\"]*[(] [^)\"]*[)][^)\"]*\")|(((^| )|\n)[(] [^)]*[)])"
//indicator, string, number, oparator
#define REG_FIND_INDI "(\"[^\"]*\")|([0-9a-zA-Z_.]+)|([^0-9a-zA-Z_. \n\t])"

