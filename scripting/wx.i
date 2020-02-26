/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012 Miguel Angel Ajo <miguelangel@nbee.es>
 * Copyright (C) 1992-2012 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file wx.i
 * @brief wx wrappers for basic things, wxString, wxPoint, wxRect, etc..
 *        all the wx objects are very complex, and we don't want to pull
 *        and swig all depending objects, so we just define the methods
 *        we want to wrap.
 */

%{
#include <wx_python_helpers.h>
%}

// encoding setup, ascii by default ///////////////////////////////////////////

void wxSetDefaultPyEncoding(const char* encoding);
const char* wxGetDefaultPyEncoding();


// wxRect class wrapper ///////////////////////////////////////////////////////

class wxRect
{
public:
    wxRect() : x(0), y(0), width(0), height(0)  { }
    wxRect(int xx, int yy, int ww, int hh): x(xx), y(yy), width(ww), height(hh) { }
    wxRect(const wxPoint& topLeft, const wxPoint& bottomRight);
    wxRect(const wxPoint& pt, const wxSize& size)
        : x(pt.x), y(pt.y), width(size.x), height(size.y) { }
    wxRect(const wxSize& size): x(0), y(0), width(size.x), height(size.y) { }

    int GetX() const { return x; }
    void SetX(int xx) { x = xx; }

    int GetY() const { return y; }
    void SetY(int yy) { y = yy; }

    int GetWidth() const { return width; }
    void SetWidth(int w) { width = w; }

    int GetHeight() const { return height; }
    void SetHeight(int h) { height = h; }

    wxPoint GetPosition() const { return wxPoint(x, y); }
    void SetPosition( const wxPoint &p ) { x = p.x; y = p.y; }

    int x, y, width, height;

    %extend
    {
       /* extend the wxRect object so it can be converted into a tuple */
       PyObject* Get()
       {
            PyObject* res = PyTuple_New(4);
            PyTuple_SET_ITEM(res, 0, PyInt_FromLong(self->x));
            PyTuple_SET_ITEM(res, 1, PyInt_FromLong(self->y));
            PyTuple_SET_ITEM(res, 2, PyInt_FromLong(self->width));
            PyTuple_SET_ITEM(res, 3, PyInt_FromLong(self->height));
            return res;
        }
    }


    %pythoncode
    {

    def __eq__(self,other):
        return self.x==other.x and self.y==other.y and self.width==other.width and self.height==other.height
    def __str__(self):                   return str(self.Get())
    def __repr__(self):                  return 'wxRect'+str(self.Get())
    def __len__(self):                   return len(self.Get())
    def __getitem__(self, index):        return self.Get()[index]
    def __setitem__(self, index, val):
        if  index == 0:     self.SetX(val)
        elif    index == 1:     self.SetY(val)
        elif    index == 2:     self.SetWidth(val)
        elif    index == 3:     self.SetHeight(val)
        else:           raise IndexError
    def __nonzero__(self):               return self.Get() != (0,0,0,0)
    __safe_for_unpickling__ = True
   }

};

// wxSize class wrapper ///////////////////////////////////////////////////////

class wxSize
{
public:
    int x,y;
    wxSize(int xx, int yy) : x(xx), y(yy) { }
    wxSize(double xx, double yy) : x(xx), y(yy) {}
    %extend
    {
       PyObject* Get()
       {
            PyObject* res = PyTuple_New(2);
            PyTuple_SET_ITEM(res, 0, PyInt_FromLong(self->x));
            PyTuple_SET_ITEM(res, 1, PyInt_FromLong(self->y));
            return res;
        }
    }

    ~wxSize();

    void SetWidth(int w);
    void SetHeight(int h);
    int GetWidth() const;
    int GetHeight() const;


    %pythoncode
    {
        def Scale(self,xscale,yscale):
            return wxSize(self.x*xscale,self.y*yscale)
        def __eq__(self,other):
            return self.GetWidth()==other.GetWidth() and self.GetHeight()==other.GetHeight()
        def __str__(self):                   return str(self.Get())
        def __repr__(self):                  return 'wxSize'+str(self.Get())
        def __len__(self):                   return len(self.Get())
        def __getitem__(self, index):        return self.Get()[index]
        def __setitem__(self, index, val):
            if  index == 0:     self.SetWidth(val)
            elif    index == 1:     self.SetHeight(val)
            else:           raise IndexError
        def __nonzero__(self):               return self.Get() != (0,0)
        __safe_for_unpickling__ = True

    }
};

// wxPoint class wrapper to (xx,yy) tuple /////////////////////////////////////

class wxPoint
{
public:
    int x, y;
    wxPoint(int xx, int yy);
    wxPoint(double xx, double yy) : x(xx), y(yy) {}
    ~wxPoint();
    %extend {
        wxPoint __add__(const wxPoint& pt) {   return *self + pt;  }
        wxPoint __sub__(const wxPoint& pt) {   return *self - pt;  }

        void Set(long x, long y) {  self->x = x;     self->y = y;  }
        PyObject* Get()
    {
            PyObject* tup = PyTuple_New(2);
            PyTuple_SET_ITEM(tup, 0, PyInt_FromLong(self->x));
            PyTuple_SET_ITEM(tup, 1, PyInt_FromLong(self->y));
            return tup;
        }
    }

    %pythoncode {
    def __eq__(self,other):            return (self.x==other.x and self.y==other.y)
    def __ne__(self,other):            return not (self==other)
    def __str__(self):                 return str(self.Get())
    def __repr__(self):                return 'wxPoint'+str(self.Get())
    def __len__(self):                 return len(self.Get())
    def __getitem__(self, index):      return self.Get()[index]
    def __setitem__(self, index, val):
        if index == 0:
            self.x = val
        elif index == 1:
            self.y = val
        else:
            raise IndexError
    def __nonzero__(self):               return self.Get() != (0,0)

    }
};


// wxChar typemaps ///////////////////////////////////////////////////////////

/* they handle the conversion from/to strings */

%typemap(in) wxChar {   wxString str = Py2wxString($input); $1 = str[0];   }
%typemap(out) wxChar {  wxString str($1);      $result = wx2PyString(str); }

// wxString wrappers /////////////////////////////////////////////////////////

%typemap(out) wxString&
{
%#if wxUSE_UNICODE
    $result = PyUnicode_FromWideChar($1->c_str(), $1->Len());
%#else
    $result = PyString_FromStringAndSize($1->c_str(), $1->Len());
%#endif
}

%apply wxString& { wxString* }

%typemap(out) wxString
{
%#if wxUSE_UNICODE
    $result = PyUnicode_FromWideChar($1.c_str(), $1.Len());
%#else
    $result = PyString_FromStringAndSize($1.c_str(), $1.Len());
%#endif
}

%typemap(varout) wxString
{
%#if wxUSE_UNICODE
    $result = PyUnicode_FromWideChar($1.c_str(), $1.Len());
%#else
    $result = PyString_FromStringAndSize($1.c_str(), $1.Len());
%#endif
}

%typemap(in) wxString& (bool temp=false)
{
    $1 = newWxStringFromPy($input);
    if ($1 == NULL) SWIG_fail;
    temp = true;
}

%typemap(freearg) wxString&
{
    if (temp$argnum)
        delete $1;
}


%typemap(in) wxString {
    wxString* sptr = newWxStringFromPy($input);
    if (sptr == NULL) SWIG_fail;
    $1 = *sptr;
    delete sptr;
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) wxString& {
    $1 = PyString_Check($input) || PyUnicode_Check($input);
}


// wxArrayString wrappers //////////////////////////////////////////////////////
%typemap(in) wxArrayString& (bool temp=false) {
    if (!PySequence_Check($input))
    {
        PyErr_SetString(PyExc_TypeError, "Not a sequence of strings");
        SWIG_fail;
    }

    $1 = new wxArrayString;
    temp = true;
    int last=PySequence_Length($input);
    for (int i=0; i<last; i++)
    {
        PyObject* pyStr = PySequence_GetItem($input, i);
        wxString* wxS = newWxStringFromPy(pyStr);
        if (PyErr_Occurred())
            SWIG_fail;
        $1->Add(*wxS);
        delete wxS;
        Py_DECREF(pyStr);
    }
}

%typemap(freearg) wxArrayString&
{
    if (temp$argnum)
        delete $1;
}

%typemap(out) wxArrayString&
{
    $result = wxArrayString2PyList(*$1);
}

%typemap(out) wxArrayString
{
    $result = wxArrayString2PyList($1);
}

%template(wxPoint_Vector) std::vector<wxPoint>;
