#ifndef GETID_API
#define GETID_API extern "C" _declspec(dllexport)

GETID_API bool CSC600SendCMD(int Com, char *sCommand);

#endif
