//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//   Wichtiger Hinweis zur DLL-Speicherverwaltung, falls die DLL die statische
//   Version der Laufzeitbibliothek (RTL) verwendet:
//
//   Wenn die DLL Funktionen exportiert, die String-Objekte (oder Strukturen/
//   Klassen, die verschachtelte Strings enthalten) als Parameter oder Funktionsergebnisse übergibt,
//   muß die Bibliothek MEMMGR.LIB im DLL-Projekt und anderen Projekten,
//   die die DLL verwenden, vorhanden sein. Sie benötigen MEMMGR.LIB auch dann,
//   wenn andere Projekte, die die DLL verwenden, new- oder delete-Operationen
//   auf Klassen anwenden, die nicht von TObject abgeleitet sind und die aus der DLL exportiert
//   werden. Durch das Hinzufügen von MEMMGR.LIB wird die DLL und deren aufrufende EXEs
//   angewiesen, BORLNDMM.DLL als Speicherverwaltung zu benutzen. In diesem Fall
//   sollte die Datei BORLNDMM.DLL zusammen mit der DLL weitergegeben werden.
//
//   Um die Verwendung von BORLNDMM.DLL, zu vermeiden, sollten String-Informationen als "char *" oder
//   ShortString-Parameter weitergegeben werden.
//
//   Falls die DLL die dynamische Version der RTL verwendet, müssen Sie
//   MEMMGR.LIB nicht explizit angeben.
//---------------------------------------------------------------------------
#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------
 