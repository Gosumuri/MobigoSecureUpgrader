#include "stdafx.h"
#include "GscConst2Str.h"

CGscConst2Str::CGscConst2Str(void)
{
}

CGscConst2Str::~CGscConst2Str(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
// primitive

char* CGscConst2Str::PatternNum2Str(int nPatternNum)
{
	switch (nPatternNum) {
		case 10: return "samsung_10";
		case 12: return "samsung_122_123_124_125";
		case 13: return "samsung_13";
		case 16: return "samsung_16";
		case 17: return "samsung_17";
		case 18: return "SAMSUNG_HSP_SYNCML";
		case 19: return "samsung_19";
		case 20: return "SAMSUNG_HSP_SYNCML";
		case 22: return "SAMSUNG_HSP_SYNCML";
		case 23: return "SAMSUNG_HSP_SYNCML";
		case 26: return "samsung_26";
		case 27: return "samsung_27";
		case 28: return "samsung_28";
		case 29: return "samsung_29";
		case 30: return "samsung_30";
		case 31: return "C_Nain";
		case 37: return "LG_37";
		case 38: return "LG_38";
		case 39: return "LG_39";
		case 40: return "LG_40";
		case 41: return "LG_41";
		case 42: return "LG_42";
		case 43: return "LG_44";
		case 44: return "LG_44";
		case 45: return "LG_45";
		case 46: return "LG_46";
		case 47: return "LG_47";
		case 48: return "LG_48";
		case 49: return "LG_49";
		case 50: return "Sky_50";
		case 51: return "Sky_51";
		case 52: return "Sky_52";
		case 60: return "Sky_60";
		case 61: return "Sky_61";
		case 62: return "Sky_62";
		case 64: return "Sky_64";
		case 66: return "Sky_66";
		case 67: return "sky_67";
		case 68: return "sky_68";
		case 69: return "sky_69";
		case 70: return "sky_70";
		case 71: return "sky_71";
		case 72: return "sky_72";
		case 73: return "sky_73";
		case 81: return "NEON_081";
		case 82: return "NEON_082";
		case 83: return "NEON_083";
		case 84: return "EVER_084";
		case 85: return "EVER_085";
		case 86: return "EVER_086";
		case 87: return "EVER_087";
		case 88: return "EVER_088";
		case 89: return "EVER_089";
		case 90: return "EVER_090";
		case 91: return "EVER_091";
		case 92: return "EVER_092";
		case 93: return "EVER_093";
		case 94: return "EVER_094";
		case 95: return "EVER_095";
		case 96: return "Moto_096";
		case 97: return "Moto_098";
		case 98: return "Moto_098";
		case 99: return "Moto_099";
		case 100: return "Moto_100";
		case 101: return "Moto_101";
		case 1018: return "SAMSUNG_HSP_SYNCML";
		case 1022: return "SAMSUNG_HSP_SYNCML";
		case 1023: return "SAMSUNG_HSP_SYNCML";
		case 1026: return "samsung_1026";
		case 1027: return "samsung_1027";
		case 1028: return "samsung_1028";
		case 1029: return "samsung_1029";
		case 103: return "Moto_103";
		case 1030: return "samsung_1030";
		case 104: return "Moto_104";
		case 105: return "Moto_105";
		case 106: return "Moto_106";
		case 107: return "Moto_107";
		case 1071: return "sky_1071";
		case 1073: return "sky_1073";
		case 108: return "Moto_108";
		case 109: return "Moto_109";
		case 1090: return "EVER_1090";
		case 1091: return "EVER_1091";
		case 1092: return "EVER_1092";
		case 1094: return "EVER_1094";
		case 1095: return "EVER_1095";
		case 1096: return "Moto_1096";
		case 1097: return "Moto_1097";
		case 1098: return "Moto_1098";
		case 1099: return "Moto_1099";
		case 110: return "Hundai_110";
		case 1100: return "Moto_1100";
		case 1106: return "Moto_1106";
		case 1108: return "Moto_1108";
		case 1109: return "Moto_1109";
		case 111: return "Hundai_111";
		case 112: return "Hundai_112";
		case 113: return "Hundai_113";
		case 114: return "Hundai_114";
		case 115: return "Hundai_115";
		case 116: return "Hundai_116";
		case 1162: return "PentechCuritel_1162";
		case 1163: return "PentechCuritel_1163";
		case 117: return "Hundai_117";
		case 118: return "Hundai_118";
		case 119: return "Hundai_119";
		case 120: return "Sewon_120";
		case 121: return "Sewon_121";
		case 122: return "EX7000_122";
		case 123: return "Sewon_123";
		case 124: return "Sewon_124";
		case 126: return "CoCo_126";
		case 128: return "PantechCuritel_128";
		case 129: return "PantechCuritel_129";
		case 130: return "PentechCuritel_130";
		case 131: return "Hundai_131";
		case 132: return "Hundai_132";
		case 133: return "Hundai_133";
		case 134: return "Hundai_134";
		case 135: return "Hundai_135";
		case 136: return "Hundai_136";
		case 137: return "Hundai_137";
		case 138: return "Hundai_138";
		case 1383: return "PDA_1383";
		case 139: return "Hundai_139";
		case 140: return "Hundai_140";
		case 1400: return "NOKIA";
		case 141: return "Hundai_133";
		case 142: return "Moto_142";
		case 143: return "Hundai_143";
		case 144: return "Hundai_144";
		case 1454: return "LG_1454";
		case 1455: return "LG_1455";
		case 1456: return "LG_1456";
		case 146: return "Hundai_146";
		case 1467: return "sky_1467";
		case 147: return "Hundai_147";
		case 148: return "Hundai_148";
		case 149: return "Hundai_149";
		case 1493: return "EVER_1493";
		case 1494: return "EVER_1494";
		case 1495: return "EVER_1495";
		case 1496: return "EVER_1496";
		case 1497: return "EVER_1497";
		case 1498: return "Moto_1498";
		case 150: return "PentechCuritel_150";
		case 151: return "Hundai_151";
		case 152: return "Hundai_152";
		case 153: return "Hundai_153";
		case 154: return "Hundai_154";
		case 155: return "PentechCuritel_155";
		case 156: return "Hundai_156";
		case 157: return "Hundai_157";
		case 158: return "Hundai_158";
		case 159: return "Hundai_159";
		case 1596: return "Moto_1596";
		case 160: return "VK_160";
		case 161: return "VK_161";
		case 162: return "PentechCuritel_162";
		case 163: return "PentechCuritel_163";
		case 1630: return "SKTelesys";
		case 310: return "PDA_COMMON";
		case 311: return "PDA_MC7000";
		case 320: return "PDA_U250";
		case 330: return "PDA_POZ";
		case 331: return "PDA_POZ2";
		case 340: return "PDA_JATY";
		case 350: return "PDA_LG8000";
		case 351: return "PDA_LG8000_2";
		case 355: return "PDA_HP6100";
		case 370: return "PDA_M4300";
		case 380: return "PDA_M8000T";
		case 381: return "PDA_PBOOK_BY_ML";
		case 382: return "PDA_PBOOK_BY_ML";
		case 383: return "PDA_PBOOK_BY_ML";
		case 400: return "NOKIA";
		case 401: return "NOKIA_LUMIA";
		case 447: return "LG_447";
		case 448: return "LG_448";
		case 449: return "LG_449";
		case 450: return "LG_450";
		case 451: return "LG_451";
		case 454: return "LG_445";
		case 455: return "LG_455";
		case 456: return "LG_456";
		case 457: return "LG_USB_MODEM";
		case 467: return "sky_467";
		case 468: return "sky_67";
		case 469: return "sky_469";
		case 470: return "SKYP2100_470";
		case 493: return "EVER_493";
		case 494: return "EVER_494";
		case 495: return "EVER_495";
		case 496: return "EVER_496";
		case 497: return "EVER_494";
		case 498: return "Moto_498";
		case 500: return "Hundai_500";
		case 501: return "Hundai_501";
		case 502: return "Hundai_502";
		case 520: return "AMTelelcom";
		case 540: return "SKMTEK_540";
		case 553: return "Hundai_553";
		case 596: return "Moto_096";
		case 600: return "KT_ANN";
		case 610: return "Motoroi_610";
		case 611: return "OBEX_Sample";
		case 612: return "OBEX_Sample";
		case 613: return "OBEX_Sample";
		case 615: return "SFX_AndroidBase";
		case 620: return "OBEX_Sample";
		case 621: return "OBEX_Sample";
		case 625: return "OBEX_Sample";
		case 630: return "SKTelesys";
		case 650: return "mp650";
		case 670: return "APPLE_670";
		case 672: return "APPLE_672";
		case 701: return "ModelTest_SAMSUNG";
		case 702: return "ModelTest_MOTOROLA";
		case 703: return "ModelTest_LG";
		case 704: return "ModelTest_KTF";
		case 705: return "ModelTest_PantechCuritel";
		case 706: return "ModelTest_Sewon";
		case 707: return "ModelTest_SKY";
		case 708: return "ModelTest_Telson";
		case 709: return "ModelTest_VK";
		case 710: return "ModelTest_Sejin";
		case 711: return "ModelTest_PDA";
		case 712: return "ModelTest_KTAnn";
		case 713: return "ModelTest_NOKIA";
		case 714: return "ModelTest_AMT //(AMTelecom)";
		case 715: return "ModelTest_Sewon";
		case 716: return "ModelTest_BLACKBERRY";
		case 717: return "ModelTest_SKTelesys";
		case 718: return "ModelTest_APPLE";
		case 719: return "ModelTest_HTC";
		case 720: return "ModelTest_SONY_ERICSSON";
		case 721: return "ModelTest_DELL";
		case 722: return "ModelTest_Sewon";
		case 723: return "ModelTest_IRIVER";
		case 724: return "ModelTest_CASIO";
		case 725: return "ModelTest_UTCOM";
		case 726: return "ModelTest_ETC";
		case 727: return "ModelTest_SAMSUNG";
		case 750: return "DetectConnectedVendorIndex";
		case 760: return "AutoFind_VendorIndex";
		case 810: return "Blackberry";
		case 818: return "SHOP_MATCH_ONLY";
		case 920: return "COPY_CSVFILE_USERFOLDER";
		case 930: return "DoAppleActivation";
		case 940: return "UploadToTCloudServer";
		case 990: return "READ_DATA_VIEW";
		case 991: return "CALL_SMS_MASH_UP";
		case 992: return "AFTER_INIT";
		case 993: return "CHECK_PAY_HWVOLUME";
		case 994: return "READ_SERVER";
		case 995: return "AFTER_WRITE_PATTERN";
		case 996: return "SAVE_SERVER";
		case 997: return "Before_RW_pattern";
		case 998: return "PRINT";
		case 999: return "SelectModel";
		case 32000: return "INTERNAL_CONFIG_PATTERN_NUM";
		case 7902: return "AFTER_PHOTO_INIT";
		case 7901: return "BEFORE_RW_PATTERN_MPHOTO";
		case 7903: return "AFTER_WRITE_PATTERN_MPHOTO";
	}

	static char szUnknown[128];
	memset(szUnknown, 0, sizeof(szUnknown));
	sprintf(szUnknown, "OP_PATTERN_UNKNOWN(%d)", nPatternNum);
	return szUnknown;
}

char* CGscConst2Str::Operation2Str(int nOperation )
{
	// NEW OPERATION
	#define OP_BASE_CODE					0	//사용하지 않는다. 범위설정용..
	#define OP_PRINT						1	//print
	#define OP_READ_SERVER					2	//read server
	#define OP_CHECK_PAY					3
	#define OP_AFTER_PIMS_INIT				4	//주소록용...
	#define OP_AFTER_PHOTO_INIT				5	//사진용...
	#define OP_SYSTEMUSE_PHOTO				6	
	#define OP_SAVE2PC_PHOTO				7
	#define OP_AFTER_BELL_INIT				8	// 벨용...
	#define OP_AFTER_BK_PHOTO_INIT			9	// 배경 사진용..
		// 10 : MobigoWiz 에서 사용하고 있어서 Skip
	#define OP_CALL_SMS_MASH_UP				11 // SMS Mash Up Contents
	#define OP_FILL_BELL_LIST				12 // Fill Bell List
	#define OP_CHECK_PAY_BELL				13
	#define OP_SEND_KT_SMS					14 // KT에서 요청한 SMS 전송 패턴
	#define OP_AFTER_SMS_INIT				15
	#define OP_IPHONE_ACTIVATION			16
	#define OP_T_CLOUD_PIMS_WRITE			17
	#define OP_T_CLOUD_PIMS_WRITE_PHOTO		18

	// 2016. 01. 28 add. w  준범선임님 모비고플러스 코드에서 가져옴
	// 추가
	#define OP_BEFORE_RW_PATTERN			19
	#define OP_AFTER_READ_PATTERN			20
	#define OP_AFTER_WRITE_PATTERN			21
	#define OP_SSP_INFO_PATTERN				22
	#define OP_SSP_ANDROID_PATTERN			23
	#define OP_END_CODE				(OP_SSP_ANDROID_PATTERN + 1)	//사용하지 않는다. 범위설정용..

	switch (nOperation)
	{
		SYM2STR_CASE_RETURN(OP_BASE_CODE);
		SYM2STR_CASE_RETURN(OP_PRINT);
		SYM2STR_CASE_RETURN(OP_READ_SERVER);
		SYM2STR_CASE_RETURN(OP_CHECK_PAY);
		SYM2STR_CASE_RETURN(OP_AFTER_PIMS_INIT);
		SYM2STR_CASE_RETURN(OP_AFTER_PHOTO_INIT);
		SYM2STR_CASE_RETURN(OP_SYSTEMUSE_PHOTO);
		SYM2STR_CASE_RETURN(OP_SAVE2PC_PHOTO);
		SYM2STR_CASE_RETURN(OP_AFTER_BELL_INIT);
		SYM2STR_CASE_RETURN(OP_AFTER_BK_PHOTO_INIT);
		SYM2STR_CASE_RETURN(OP_CALL_SMS_MASH_UP);
		SYM2STR_CASE_RETURN(OP_FILL_BELL_LIST);
		SYM2STR_CASE_RETURN(OP_CHECK_PAY_BELL);
		SYM2STR_CASE_RETURN(OP_SEND_KT_SMS);
		SYM2STR_CASE_RETURN(OP_AFTER_SMS_INIT);
		SYM2STR_CASE_RETURN(OP_IPHONE_ACTIVATION);
		SYM2STR_CASE_RETURN(OP_T_CLOUD_PIMS_WRITE);
		SYM2STR_CASE_RETURN(OP_T_CLOUD_PIMS_WRITE_PHOTO);
		SYM2STR_CASE_RETURN(OP_BEFORE_RW_PATTERN);
		SYM2STR_CASE_RETURN(OP_AFTER_READ_PATTERN);
		SYM2STR_CASE_RETURN(OP_AFTER_WRITE_PATTERN);
		SYM2STR_CASE_RETURN(OP_SSP_INFO_PATTERN);
		SYM2STR_CASE_RETURN(OP_SSP_ANDROID_PATTERN);
		SYM2STR_CASE_RETURN(OP_END_CODE);
	}
	static char szUnknown[128];
	memset(szUnknown, 0, sizeof(szUnknown));
	sprintf(szUnknown, "OP_UNKNOWN(%d)", nOperation);
	return szUnknown;
}


char* CGscConst2Str::VendorID2Str(int nVendorID )
{
	#define VENDOR_SAMSUNG			1
	#define VENDOR_LGE				2
	#define VENDOR_SKTT				3
	#define VENDOR_KTFT				4
	#define VENDOR_MOTOROLA			5
	#define VENDOR_PNCURITEL		6
	#define VENDOR_SEWON			7
	#define VENDOR_TELSON			8
	#define VENDOR_INTERCUBE		9
	#define VENDOR_CASIO			10
	#define VENDOR_ETC				11
	#define VENDOR_VK				12
	#define VENDOR_SEJIN			13
	#define VENDOR_PDA				14
	#define VENDOR_AUTO				15
	#define VENDOR_SKT_UNI			16
	#define VENDOR_PNC_OBEX_DRV		17
	#define	VENDOR_KT_ANN			18
	#define VENDOR_SIM_USIM			19
	#define VENDOR_LG_POWER_COMM	20
	#define VENDOR_NOKIA			21
	#define VENDOR_BLACKBERRY		22
	#define VENDOR_KT_POWERTEL		23
	#define VENDOR_MESSAGE_SERIAL	24
	#define VENDOR_SKTELESYS		25
	#define VENDOR_APPLE			26
	#define VENDOR_HTC				27
	#define VENDOR_GOOGLE			28
	#define VENDOR_SONY_ERICSSON	29
	#define VENDOR_DELL				30
	#define VENDOR_IRIVER			31
	#define VENDOR_UTCOM			32
	#define VENDOR_RESERVED			33

	switch (nVendorID)
	{
		SYM2STR_CASE_RETURN(VENDOR_SAMSUNG);
		SYM2STR_CASE_RETURN(VENDOR_LGE);
		SYM2STR_CASE_RETURN(VENDOR_SKTT);
		SYM2STR_CASE_RETURN(VENDOR_KTFT);
		SYM2STR_CASE_RETURN(VENDOR_MOTOROLA);
		SYM2STR_CASE_RETURN(VENDOR_PNCURITEL);
		SYM2STR_CASE_RETURN(VENDOR_SEWON);
		SYM2STR_CASE_RETURN(VENDOR_TELSON);
		SYM2STR_CASE_RETURN(VENDOR_INTERCUBE);
		SYM2STR_CASE_RETURN(VENDOR_CASIO);
		SYM2STR_CASE_RETURN(VENDOR_ETC);
		SYM2STR_CASE_RETURN(VENDOR_VK);
		SYM2STR_CASE_RETURN(VENDOR_SEJIN);
		SYM2STR_CASE_RETURN(VENDOR_PDA);
		SYM2STR_CASE_RETURN(VENDOR_AUTO);
		SYM2STR_CASE_RETURN(VENDOR_SKT_UNI);
		SYM2STR_CASE_RETURN(VENDOR_PNC_OBEX_DRV);
		SYM2STR_CASE_RETURN(VENDOR_KT_ANN);
		SYM2STR_CASE_RETURN(VENDOR_SIM_USIM);
		SYM2STR_CASE_RETURN(VENDOR_LG_POWER_COMM);
		SYM2STR_CASE_RETURN(VENDOR_NOKIA);
		SYM2STR_CASE_RETURN(VENDOR_BLACKBERRY);
		SYM2STR_CASE_RETURN(VENDOR_KT_POWERTEL);
		SYM2STR_CASE_RETURN(VENDOR_MESSAGE_SERIAL);
		SYM2STR_CASE_RETURN(VENDOR_SKTELESYS);
		SYM2STR_CASE_RETURN(VENDOR_APPLE);
		SYM2STR_CASE_RETURN(VENDOR_HTC);
		SYM2STR_CASE_RETURN(VENDOR_GOOGLE);
		SYM2STR_CASE_RETURN(VENDOR_SONY_ERICSSON);
		SYM2STR_CASE_RETURN(VENDOR_DELL);
		SYM2STR_CASE_RETURN(VENDOR_IRIVER);
		SYM2STR_CASE_RETURN(VENDOR_UTCOM);
		SYM2STR_CASE_RETURN(VENDOR_RESERVED);
	}
	static char szUnknown[128];
	memset(szUnknown, 0, sizeof(szUnknown));
	sprintf(szUnknown, "VENDER_UNKNOWN(%d)", nVendorID);
	return szUnknown;
}