# 
# vitamtp spec file
# 

%define _version 2.5.4

%if "%{_version}" == "testing" || "%{_version}" == "master"
%define _verprefix
%else
%define _verprefix v
%endif

Name:           libvitamtp
Version:        %{_version}
Release:        0
%define sonum   4
Summary:        Low-level Vita communication library
License:        GPL-3.0
Group:          System/Libraries
URL:            https://github.com/codestation/vitamtp
Source:         https://github.com/codestation/vitamtp/archive/%{_verprefix}%{_version}/vitamtp-%{_version}.tar.gz
BuildRequires:  pkgconfig
BuildRequires:  libxml2-devel
BuildRequires:  libusbx-devel

%description
libvitamtp is a library based off of libmtp that does low level USB
communications with the Vita. It can read and receive MTP commands that
the Vita sends, which are a proprietary set of commands that is based on
the MTP open standard.

%package -n %{name}%{sonum}
Summary:        Low-level Vita communication library
Group:          System/Libraries

%description -n %{name}%{sonum}
libvitamtp is a library based off of libmtp that does low level USB
communications with the Vita. It can read and receive MTP commands that
the Vita sends, which are a proprietary set of commands that is based on
the MTP open standard.

%package devel
Summary:        Low-level Vita communication library - development files
Group:          Development/Libraries/C and C++
Requires:       %{name}%{sonum} = %{version}
Requires:       libxml2-devel
Requires:       libusbx-devel

%description devel
libvitamtp is a library based off of libmtp that does low level USB
communications with the Vita. It can read and receive MTP commands that
the Vita sends, which are a proprietary set of commands that is based on
the MTP open standard.
This package contains only the files necessary for development.

%setup -n %{name}%{sonum} -DT

%build
./autogen.sh
./configure --prefix=/usr --libdir=%{_libdir}
make %{?_smp_mflags}

%install
%makeinstall
rm -rf %{buildroot}/%{_libdir}/*.la
mkdir -p %{buildroot}/usr/lib/udev/rules.d
cp debian/vitamtp%{sonum}.udev %{buildroot}/usr/lib/udev/rules.d/80-psvita.rules

%post -n %{name}%{sonum} -p /sbin/ldconfig
%postun -n %{name}%{sonum} -p /sbin/ldconfig

%files -n %{name}%{sonum}
%defattr(-,root,root)
%doc README.md ChangeLog COPYING
%{_libdir}/lib*.so.*
%{_udevrulesdir}/80-psvita.rules

%files devel
%defattr(-,root,root)
%{_prefix}/include/vitamtp.h
%{_libdir}/libvitamtp.a
%{_libdir}/libvitamtp.so
%{_libdir}/pkgconfig/libvitamtp.pc

%changelog
