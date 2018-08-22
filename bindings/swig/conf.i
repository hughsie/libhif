%module(directors="1") conf

%include <stdint.i>
%include <std_map.i>
%include <std_pair.i>
%include <std_vector.i>
%include <std_string.i>

%begin %{
    #define SWIG_PYTHON_2_UNICODE
%}

%{
    // make SWIG wrap following headers
    #include <iterator>
    #include "libdnf/conf/ConfigRepo.hpp"
    #include "libdnf/conf/ConfigParser.hpp"
    #include "libdnf/repo/Repo.hpp"
    typedef struct {
        PyObject_HEAD
        HyRepo repo;
    } _RepoObject;
    using namespace libdnf;
%}

%typemap(in) HyRepo {
    $1 = ((_RepoObject *)$input)->repo;
}

%include <exception.i>
%exception {
    try {
        $action
    }
    catch (const std::exception & e)
    {
       SWIG_exception(SWIG_RuntimeError, (std::string("C++ std::exception: ") + e.what()).c_str());
    }
    catch (...)
    {
       SWIG_exception(SWIG_UnknownError, "C++ anonymous exception");
    }
}

// for SWIG < 3.0.0
//#define override
//#define noexcept

// make SWIG look into following headers
%include "libdnf/conf/Option.hpp"
%include "libdnf/conf/OptionChild.hpp"
%include "libdnf/conf/OptionBool.hpp"
%include "libdnf/conf/OptionEnum.hpp"
%template(OptionEnumString) libdnf::OptionEnum<std::string>;
%include "libdnf/conf/OptionNumber.hpp"
%template(OptionNumberInt32) libdnf::OptionNumber<std::int32_t>;
%template(OptionNumberUInt32) libdnf::OptionNumber<std::uint32_t>;
%template(OptionNumberInt64) libdnf::OptionNumber<std::int64_t>;
%template(OptionNumberUInt64) libdnf::OptionNumber<std::uint64_t>;
%template(OptionNumberFloat) libdnf::OptionNumber<float>;
%include "libdnf/conf/OptionSeconds.hpp"
%include "libdnf/conf/OptionString.hpp"
%include "libdnf/conf/OptionStringList.hpp"
%include "libdnf/conf/OptionStringListAppend.hpp"
%include "libdnf/conf/OptionPath.hpp"

%template(OptionChildBool) libdnf::OptionChild<OptionBool>;
%template(OptionChildString) libdnf::OptionChild<OptionString>;
%template(OptionChildStringList) libdnf::OptionChild<OptionStringList>;
%template(OptionChildNumberInt32) libdnf::OptionChild< OptionNumber<std::int32_t> >;
%template(OptionChildNumberUInt32) libdnf::OptionChild< OptionNumber<std::uint32_t> >;
%template(OptionChildNumberFloat) libdnf::OptionChild< OptionNumber<float> >;
%template(OptionChildEnumString) libdnf::OptionChild< OptionEnum<std::string> >;
%template(OptionChildSeconds) libdnf::OptionChild<OptionSeconds>;
%template(VectorString) std::vector<std::string>;
%template(PairStringString) std::pair<std::string, std::string>;
%template(MapStringString) std::map<std::string, std::string>;

%inline %{
class StopIterator {};

template<class T>
class Iterator {
public:
    Iterator(typename T::iterator _cur, typename T::iterator _end) : cur(_cur), end(_end) {}
    Iterator* __iter__()
    {
      return this;
    }

    typename T::iterator cur;
    typename T::iterator end;
  };
%}

%include "libdnf/conf/Config.hpp"
%include "libdnf/conf/ConfigMain.hpp"
%include "libdnf/conf/ConfigRepo.hpp"

%template(OptionBindsIterator) Iterator<libdnf::OptionBinds>;

%exception libdnf::ConfigParser::read {
    try
    {
        $action
    }
    catch (const libdnf::ConfigParser::CantOpenFile & e)
    {
        SWIG_exception(SWIG_IOError, e.what());
    }
    catch (const libdnf::ConfigParser::Exception & e)
    {
       SWIG_exception(SWIG_RuntimeError, e.what());
    }
    catch (const std::exception & e)
    {
       SWIG_exception(SWIG_RuntimeError, (std::string("C++ std::exception: ") + e.what()).c_str());
    }
}

%exception libdnf::ConfigParser::getValue {
    try
    {
        $action
    }
    catch (const libdnf::ConfigParser::MissingSection & e)
    {
        SWIG_exception(SWIG_IndexError, e.what());
    }
    catch (const libdnf::ConfigParser::MissingOption & e)
    {
        SWIG_exception(SWIG_IndexError, e.what());
    }
    catch (const libdnf::ConfigParser::Exception & e)
    {
       SWIG_exception(SWIG_RuntimeError, e.what());
    }
    catch (const std::exception & e)
    {
       SWIG_exception(SWIG_RuntimeError, (std::string("C++ std::exception: ") + e.what()).c_str());
    }
}

%exception libdnf::ConfigParser::getSubstitutedValue {
    try
    {
        $action
    }
    catch (const libdnf::ConfigParser::MissingSection & e)
    {
        SWIG_exception(SWIG_IndexError, e.what());
    }
    catch (const libdnf::ConfigParser::MissingOption & e)
    {
        SWIG_exception(SWIG_IndexError, e.what());
    }
    catch (const libdnf::ConfigParser::Exception & e)
    {
       SWIG_exception(SWIG_RuntimeError, e.what());
    }
    catch (const std::exception & e)
    {
       SWIG_exception(SWIG_RuntimeError, (std::string("C++ std::exception: ") + e.what()).c_str());
    }
}

%apply std::string & INOUT { std::string & text }
%include "libdnf/conf/ConfigParser.hpp"
%clear std::string & text;
%template(MapStringMapStringString) std::map<std::string, std::map<std::string, std::string>>;

%extend libdnf::Repo {
    Repo(const std::string & id, ConfigRepo * config)
    {
        return new Repo(id, std::unique_ptr<libdnf::ConfigRepo>(config));
    }
    void setCallbacks(RepoCB * callbacks)
    {
        self->setCallbacks(std::unique_ptr<libdnf::RepoCB>(callbacks));
    }
}
%ignore libdnf::Repo::Repo;
%ignore libdnf::Repo::setCallbacks;
%feature("director") libdnf::RepoCB;
%ignore libdnf::PackageTarget::PackageTarget(const PackageTarget & src);
%template(VectorPPackageTarget) std::vector<libdnf::PackageTarget *>;
%feature("director") libdnf::PackageTargetCB;
%include "libdnf/repo/Repo.hpp"

%exception __next__() {
    try
    {
        $action  // calls %extend function next() below
    }
    catch (StopIterator)
    {
        PyErr_SetString(PyExc_StopIteration, "End of iterator");
        return NULL;
    }
}

// For old Python
%exception next() {
    try
    {
        $action  // calls %extend function next() below
    }
    catch (StopIterator)
    {
        PyErr_SetString(PyExc_StopIteration, "End of iterator");
        return NULL;
    }
}

%template(PairStringOptionBinding) std::pair<std::string, libdnf::OptionBinding *>;
%extend Iterator<libdnf::OptionBinds> {
    std::pair<std::string, libdnf::OptionBinding *> __next__()
    {
        if ($self->cur != $self->end) {
            auto & id = $self->cur->first;
            auto pValue = &($self->cur++)->second;
            return {id, pValue};
        }
        throw StopIterator();
    }
    std::pair<std::string, libdnf::OptionBinding *> next()
    {
        if ($self->cur != $self->end) {
            auto & id = $self->cur->first;
            auto pValue = &($self->cur++)->second;
            return {id, pValue};
        }
        throw StopIterator();
    }
}

%extend libdnf::OptionBinds {
    Iterator<libdnf::OptionBinds> __iter__()
    {
        return Iterator<libdnf::OptionBinds>($self->begin(), $self->end());
    }
}

%pythoncode %{
# Compatible name aliases
ConfigMain.exclude = ConfigMain.excludepkgs
%}
