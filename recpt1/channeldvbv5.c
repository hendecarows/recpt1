/* -*- tab-width: 4; indent-tabs-mode: nil -*- */
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "ini_file.h"
#include "channeldvbv5.h"

// libdvbv5形式のチャンネルからrecpt1形式のチャンネルに変換する
// 戻り値
// エラー : 負の数
// 成功   : 正の数(recpt1形式チャンネルバッファに書き込む予定だっだ文字数)
int channel_dvbv5_to_recpt1(const char *dvbv5_config, const char *dvbv5_channel, char *recpt1_channel_buf, size_t recpt1_channel_buf_size)
{
    Ini_File *ini = NULL;
    int code = -1;
    int size = 0;
    char *system = NULL;
    unsigned long value = 0;
    int ch = 0;

    // recpt1形式チャンネルバッファの必要サイズは、ISDB-Tで13-62、ISDB-SはTSIDに変換することから、
    // TSIDの最大値が 0xffff = 65535 となるため、終端null文字を加えたて 5 + 1 = 6
    if (dvbv5_config == NULL || dvbv5_channel == NULL
        || recpt1_channel_buf == NULL || recpt1_channel_buf_size < 6
    ) {
        return code;
    }

    ini = ini_file_parse(dvbv5_config, NULL);
    if (ini == NULL) {
        fprintf(stderr, "failed to parse ini file: %s\n", dvbv5_config);
        return code;
    }

    if (ini_file_find_property(ini, dvbv5_channel, "DELIVERY_SYSTEM", &system) == ini_no_error) {
        if (strcasecmp(system, "ISDBT") == 0) {
            if (ini_file_find_unsigned(ini, dvbv5_channel, "FREQUENCY", &value) == ini_no_error) {
                // ISDB-Tはdvbv5のFREQUENCYの値からrecpt1形式の13-62の文字列に変換する
                // 周波数とチャンネル番号の変換はdvbv5形式はHz単位で設定されているので、MHz単位に変換し
                // 13ch の 473 から 6 間隔で増加することを用いてチャンネル番号を算出する
                ch = ((value / 1000000) - 473) / 6 + 13;
                if (ch >= 13 && ch <= 62) {
                    size = snprintf(recpt1_channel_buf, recpt1_channel_buf_size, "%d", ch);
                    code = (int)size;
                }
            }
        } else if (strcasecmp(system, "ISDBS") == 0) {
            // ISDB-Sはdvbv5のSTREAM_IDの値をそのまま文字列に変換する
            if (ini_file_find_unsigned(ini, dvbv5_channel, "STREAM_ID", &value) == ini_no_error) {
                size = snprintf(recpt1_channel_buf, recpt1_channel_buf_size, "%d", (int)(value & 0xffff));
                code = (int)size;
            }
        }
    }

    if (ini) {
        ini_file_free(ini);
    }
    return code;
}
