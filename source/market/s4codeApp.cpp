#include "market/s4codeApp.h"
#include "common/s4exceptions.h"
#include "types/s4convertors.h"
#include "common/s4logger.h"
#define  SH_CODE_MIN 600000
#define  SH_CODE_MAX 699999
#define  SZ_CODE_MIN 000000
#define  SZ_CODE_MAX 399999

using namespace std;
//"600997" : "sh600997", "000001":"sz000001"
std::string pureCodeStr_to_mktCodeStr(const std::string& pureCode)
{
	if (pureCode.c_str()[0] == '6') {
		return move(std::string("sh").append(pureCode));
	}
	else {
		return move(std::string("sz").append(pureCode));
	}
}
//"600997" : 170600997, "000001": 330000001
mktCode_t pureCodeStr_to_mktCode(const std::string& pureCode)
{
	int c = 0;
	for (int i = 0; i < pureCode.size(); ++i) {
		c = c * 10 + pureCode.c_str()[i] - '0';
	}
	return pureCodeInt_to_mktCodeInt(c);
}
//600997 : "sh600997", 1:"sz000001"
std::string pureCodeInt_to_mktCodeStr(const int pureCode)
{
	char c[9];
	if (pureCode >= SH_CODE_MIN) {
		sprintf_s(c, "sh%06d", pureCode);
	}
	else {
		sprintf_s(c, "sz%06d", pureCode);
	}
	return move(std::string(c));
}

#pragma warning( push )
#pragma warning( disable : 4702 )   //unreachable returen
//600997 : 170600997, 1: 330000001
int pureCodeInt_to_mktCodeInt(const int pureCode)
{
	if (pureCode >= SH_CODE_MIN && pureCode<=SH_CODE_MAX) {
		return SH_PRB + pureCode;
	}
	else if(pureCode >= SZ_CODE_MIN && pureCode <= SZ_CODE_MAX){
		return SZ_PRB + pureCode;
	}
	throw CodeNameConvertError("unknow market for pureCode:"+ std::to_string(pureCode));
	return pureCode;
}
#pragma warning( pop )

int mktCodeStr_to_mktCodeInt(const std::string& mktCode)
{
	int c = 0;
	for (int i = 2; i < mktCode.size(); ++i) {
		c = c * 10 + mktCode.c_str()[i] - '0';
	}
	if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'h') {
		return SH_PRB + c;
	}
	else 
	if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'z') {
		return SZ_PRB + c;
	}
	else{
			throw CodeNameConvertError("unknow code :" + mktCode);
	}
}
std::string mktCodeInt_to_mktCodeStr(int mktCode)
{
	int c;
	char ch[9];
	if (mktCode >= SH_PRB && mktCode <= SH_PRB + PRB_MK) {
		c = mktCode - SH_PRB;
		sprintf_s(ch, "sh%06d", c);
	}
	else if (mktCode >= SZ_PRB && mktCode <= SZ_PRB + PRB_MK) {
		c = mktCode - SZ_PRB;
		sprintf_s(ch, "sz%06d", c);
	}
	else {
		throw CodeNameConvertError("unkonw code :" + IntConvertor::convert(mktCode));
	}
	return move(std::string(ch));
}

//"sh600997" : "600997", "sz000001":"000001"; [ "sh000001":ERROR ]
std::string mktCodeStr_to_pureCodeStr(const std::string& mktCode)
{
	if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'h') {
		if (mktCode.c_str()[3] == '6') {
			return move(mktCode.substr(2));
		}
		else {
			throw CodeNameConvertError("sh code error:" + mktCode);
		}
	}
	else if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'z') {
		if (mktCode.c_str()[3] == '0' || mktCode.c_str()[3] == '3') {
			return move(mktCode.substr(2));
		}
		else {
			throw CodeNameConvertError("sz code error:" + mktCode);
		}
	}
	else {
		throw CodeNameConvertError("unkonw code :" + mktCode);
	}
}
//"sh600997" : 600997, "sz000001":1; [ "sh000001":ERROR ]
int mktCodeStr_to_pureCodeInt(const std::string& mktCode)
{
	int c = 0;
	for (int i = 2; i < mktCode.size(); ++i) {
		c = c * 10 + mktCode.c_str()[i] - '0';
	}
	if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'h') {
		if (c>=SH_CODE_MIN) {
			return c;
		}
		else {
			throw CodeNameConvertError("sh code error:" + mktCode);
		}
	}
	else if (mktCode.c_str()[0] == 's' && mktCode.c_str()[1] == 'z') {
		if (c<SH_CODE_MIN) {
			return c;
		}
		else {
			throw CodeNameConvertError("sz code error:" + mktCode);
		}
	}
	else {
		throw CodeNameConvertError("unkonw code :" + mktCode);
	}

}
//170600997 : "600997", 330000001: "000001"
std::string mktCodeInt_to_pureCodeStr(const int mktCode)
{
	int c;
	char ch[7];
	if (mktCode >= SH_PRB && mktCode <= SH_PRB + PRB_MK) {
		c = mktCode - SH_PRB;
		if (c >= SH_CODE_MIN) {
			sprintf_s(ch, "%06d", c);
		}
		else {
			throw CodeNameConvertError("sh code error:" + IntConvertor::convert(mktCode));
		}
	}
	else if (mktCode >= SZ_PRB && mktCode <= SZ_PRB + PRB_MK) {
		c = mktCode - SZ_PRB;
		if (c < SH_CODE_MIN) {
			sprintf_s(ch, "%06d", c);
		}
		else {
			throw CodeNameConvertError("sz code error:" + IntConvertor::convert(mktCode));
		}
	}
	else {
		throw CodeNameConvertError("unkonw code :" + IntConvertor::convert(mktCode));
	}
	return move(std::string(ch));
}
//170600997 : 600997, 330000001: 1
int mktCodeInt_to_pureCodeInt(const int mktCode)
{
	int c;
	if (mktCode >= SH_PRB && mktCode <= SH_PRB + PRB_MK) {
		c = mktCode - SH_PRB;
		if (c < SH_CODE_MIN) {
			throw CodeNameConvertError("sh code error:" + IntConvertor::convert(mktCode));
		}
	}
	else if (mktCode >= SZ_PRB && mktCode <= SZ_PRB + PRB_MK) {
		c = mktCode - SZ_PRB;
		if (c >= SH_CODE_MIN) {
			throw CodeNameConvertError("sz code error:" + IntConvertor::convert(mktCode));
		}
	}
	else {
		throw CodeNameConvertError("unkonw code :" + IntConvertor::convert(mktCode));
	}
	return c;
	
}
/*
SH:
001��������ծ�ֻ���
110������
120��������ҵծȯ��
129������
100��������ת��ծȯ��
201��������ծ�ع���
310��������ծ�ڻ���
500������
550����������
600������A�ɣ�
700��������ɣ�
710������ת��ɣ�
701������ת�������ɣ�
711������ת�����ת��ɣ�
720������������
730�������¹��깺��
735�������»����깺��
737�������¹����ۣ�
900������B��

SZ:
��1λ	�ڶ�λ	��3-6λ	����
  0       0       XXXX  A��֤ȯ
  0       3       XXXX  A��A2Ȩ֤
  0       7       XXXX  A������
  0       8       XXXX  A��A1Ȩ֤
  0       9       XXXX  A��ת��
  1       0       XXXX  ��ծ�ֻ�
  1       1       XXXX  ծȯ
  1       2       XXXX  ��ת��ծȯ
  1       3       XXXX  ��ծ�ع�
  1       7       XXXX  ԭ��Ͷ�ʻ���
  1       8       XXXX  ֤ȯͶ�ʻ���
  2       0       XXXX  B��֤ȯ
  2       7       XXXX  B������
  2       8       XXXX  B��Ȩ֤
  3       0       XXXX  ��ҵ��֤ȯ
  3       7       XXXX  ��ҵ������
  3       8       XXXX  ��ҵ��Ȩ֤
  3       9       XXXX  �ۺ�ָ��/�ɷ�ָ��

*/
bool isStk(const std::string & MktCodeStr)
{
	if (MktCodeStr.size() != 8)
		return false;
	std::string leading = MktCodeStr.substr(0, 4);
	if (leading == "sz00" || leading == "sz30" || leading == "sh60")
		return true;

	return false;
}

bool isSZmkt(mktCode_t MktCodeInt)
{
	if (MktCodeInt > SZ_PRB && MktCodeInt < SZ_PRB + PRB_MK)
		return true;
	return false;
}
bool isSHmkt(mktCode_t MktCodeInt)
{
	if (MktCodeInt > SH_PRB && MktCodeInt < SH_PRB + PRB_MK)
		return true;
	return false;
}
bool isCYmkt(mktCode_t MktCodeInt)
{
	if (MktCodeInt > CY_PRB && MktCodeInt < CY_PRB + CY_PRB_MK)
		return true;
	return false;
}

bool isStk(mktCode_t MktCodeInt)
{
	if (isSHmkt(MktCodeInt)) {
		if (MktCodeInt >= SH_PRB + 600000 && MktCodeInt <= SH_PRB + 609999)
			return true;
		else
			return false;
	}
	else if (isSZmkt(MktCodeInt)) {
		if( (MktCodeInt >= SZ_PRB && MktCodeInt <= SZ_PRB + 9999) || 
			(MktCodeInt >= SZ_PRB + 300000 && MktCodeInt <= SZ_PRB + 309999) )
			return true;
		else
			return false;
	}

	return false;
}
//ָ��
bool isIdx(mktCode_t MktCodeInt)
{
	if (isSHmkt(MktCodeInt)) {
		if (MktCodeInt >= SH_PRB && MktCodeInt <= SH_PRB + 999)
			return true;
		else
			return false;
	}
	else if (isSZmkt(MktCodeInt)) {
		if ((MktCodeInt >= SZ_PRB + 390000 && MktCodeInt <= SH_PRB + 399999))
			return true;
		else
			return false;
	}

	return false;
}

bool pureCodeIsStk(int pureCode)
{
		if ((pureCode >= 600000 && pureCode <= 609999)||
			(pureCode >= 0 && pureCode <= 9999) ||
			(pureCode >= 300000 && pureCode <= 309999))
			return true;
	return false;
}