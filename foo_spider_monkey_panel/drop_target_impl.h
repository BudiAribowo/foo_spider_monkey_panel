#pragma once

#include <OleIdl.h>
#include <ShlObj.h>

_COM_SMARTPTR_TYPEDEF(IDropTargetHelper, IID_IDropTargetHelper);

class IDropTargetImpl : public IDropTarget
{
protected:
	IDropTargetHelperPtr m_dropTargetHelper;
	HWND m_hWnd;

public:
	IDropTargetImpl(HWND hWnd = nullptr) : m_hWnd(hWnd)
	{
		CoCreateInstance(CLSID_DragDropHelper, nullptr, CLSCTX_INPROC_SERVER, IID_IDropTargetHelper, (LPVOID*)&m_dropTargetHelper);
	}

	virtual ~IDropTargetImpl()
	{
		RevokeDragDrop();
	}

	HRESULT RegisterDragDrop()
	{
		return ::RegisterDragDrop(m_hWnd, this);
	}

	HRESULT RevokeDragDrop()
	{
		return ::RevokeDragDrop(m_hWnd);
	}

	void SetHWND(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	HWND GetHWND()
	{
		return m_hWnd;
	}

	// IDropTarget
	STDMETHODIMP DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		if (!pDataObj) return E_FAIL;
		if (!pdwEffect) return E_POINTER;

		HRESULT hr = S_OK;
		try
		{
			if (m_dropTargetHelper)
			{
				POINT point = { pt.x, pt.y };
				m_dropTargetHelper->DragEnter(m_hWnd, pDataObj, &point, *pdwEffect);
			}

			hr = OnDragEnter(pDataObj, grfKeyState, pt, pdwEffect);
			if (FAILED(hr))
			{
				return hr;
			}
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		if (!pdwEffect) return E_POINTER;

		HRESULT hr = S_OK;
		try
		{
			if (m_dropTargetHelper)
			{
				POINT point = { pt.x, pt.y };
				m_dropTargetHelper->DragOver(&point, *pdwEffect);
			}

			hr = OnDragOver(grfKeyState, pt, pdwEffect);
			if (FAILED(hr))
			{
				return hr;
			}
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	STDMETHODIMP DragLeave()
	{
		HRESULT hr = S_OK;

		try
		{
			if (m_dropTargetHelper)
			{
				m_dropTargetHelper->DragLeave();
			}

			hr = OnDragLeave();
			if (FAILED(hr))
			{
				return hr;
			}
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		HRESULT hr = S_OK;

		if (!pDataObj) return E_FAIL;
		if (!pdwEffect) return E_POINTER;

		try
		{
			if (m_dropTargetHelper)
			{
				POINT point = { pt.x, pt.y };
				m_dropTargetHelper->Drop(pDataObj, &point, *pdwEffect);
			}

			hr = OnDrop(pDataObj, grfKeyState, pt, pdwEffect);
			if (FAILED(hr))
			{
				return hr;
			}
		}
		catch (...)
		{
			return E_FAIL;
		}
		return hr;
	}

	// Overrides
    virtual HRESULT OnDragEnter( IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect ) = 0;
    virtual HRESULT OnDragOver( DWORD grfKeyState, POINTL pt, DWORD* pdwEffect ) = 0;
    virtual HRESULT OnDrop( IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect ) = 0;
    virtual HRESULT OnDragLeave() = 0;
};
