/*
 *  This file is part of Poedit (http://www.poedit.net)
 *
 *  Copyright (C) 2004-2008 Vaclav Slavik
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 */

#include <wx/log.h>
#include <wx/stdpaths.h>
#include <wx/image.h>

#include "icons.h"
#include "edapp.h"

#ifdef __WXGTK20__
// translates poedit item id or Tango stock item id to "legacy" GNOME id:
static wxString GetGnomeStockId(const wxString& id)
{
    #define MAP(poedit, gnome) if ( id == _T(poedit) ) return _T(gnome)

    MAP("document-open",        "gtk-open");
    MAP("document-save",        "gtk-save");
    MAP("window-close",         "gtk-close");

    MAP("document-new",         "gtk-new");
    MAP("document-properties",  "stock_edit");
    MAP("edit-delete",          "gtk-delete");

    #undef MAP

    return wxEmptyString; // no match found
}
#endif // __WXGTK20__


wxBitmap PoeditArtProvider::CreateBitmap(const wxArtID& id,
                                         const wxArtClient& client,
                                         const wxSize& size)
{
    // Note: On Unix, this code is only called as last resolt, if standard
    //       theme provider (that uses current icon theme and files from
    //       /usr/share/icons/<theme>) didn't find any matching icon.

    wxLogTrace(_T("poedit.icons"), _T("getting icon '%s'"), id.c_str());

#ifdef __WXGTK20__
    // try legacy GNOME icons from standard theme:
    wxString gnomeId = GetGnomeStockId(id);
    if ( !gnomeId.empty() )
    {
        wxLogTrace(_T("poedit.icons"), _T("-> legacy '%s'"), gnomeId.c_str());
        wxBitmap gbmp(wxArtProvider::GetBitmap(gnomeId, client, size));
        if ( gbmp.Ok() )
            return gbmp;
    }
#endif // __WXGTK20__

    wxString iconsdir =
#ifdef __WXMAC__
        wxStandardPaths::Get().GetResourcesDir() + _T("/icons");
#else
        wxGetApp().GetAppPath() + _T("/share/poedit/icons");
#endif
    if ( !wxDirExists(iconsdir) )
    {
        wxLogTrace(_T("poedit.icons"),
                   _T("icons dir %s not found"), iconsdir.c_str());
        return wxNullBitmap;
    }

    wxString icon;
    icon.Printf(_T("%s/%s.png"), iconsdir.c_str(), id.c_str());
    if ( !wxFileExists(icon) )
        return wxNullBitmap;

    wxLogTrace(_T("poedit.icons"), _T("loading from %s"), icon.c_str());
    wxBitmap bmp(wxImage(icon, wxBITMAP_TYPE_ANY));
    return bmp;
}