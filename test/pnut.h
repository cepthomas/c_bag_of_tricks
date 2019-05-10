#ifndef PNUT_H
#define PNUT_H

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <set>
#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

using namespace std;

/// Runtime stuff.
struct TestContext
{
    /// Current suite.
    const char* CurrentSuiteId;

    /// Output format is: x:xml, r:readable.
    char Format;

    /// Collected output lines.
    vector<string> OutLines;

    /// Collected property lines.
    vector<string> PropLines;

    /// 
    void Reset()
    {
        CurrentSuiteId = "???";
        Format = 'r';

        OutLines.clear();
        PropLines.clear();
    }
};

/// The class that encapsulates an individual test suite.
class TestSuite
{
protected:
    string _id;
    string _description;
    int _caseCnt;
    int _failCnt;

public:
    /// Properties.
    const string& ID() { return _id; }
    const string& Description() { return _description; }
    int CaseCnt() { return _caseCnt; }
    int FailCnt() { return _failCnt; }

    /// Constructor.
    TestSuite(const string& id, const string& desc)
    {
        _id = id;
        _description = desc;
        _caseCnt = 0;
        _failCnt = 0;
    }

    /// Record a test result line into the report.
    void RecordResult(TestContext& context, bool pass, const string& file, unsigned int line, const string& message)
    {
        ostringstream sstr;
        _caseCnt++;

        if (pass)
        {
            if (context.Format == 'x')
            {
                sstr << "        <testcase name=\"" << context.CurrentSuiteId << "." << _caseCnt << "\" classname=\"" << context.CurrentSuiteId << "\" />" << endl;
            }
        }
        else
        {
            _failCnt++;

            if (context.Format == 'x')
            {
                sstr << "        <testcase name=\"" << context.CurrentSuiteId << "." << _caseCnt << "\" classname=\"" << context.CurrentSuiteId << "\">" << endl;
                sstr << "            <failure message=\"" << file << ":" << line << " " << message << "\"></failure>" << endl;
                sstr << "        </testcase>" << endl;
            }
            else // readable
            {
                /// Output the failure string with file/line.
                sstr << "! (" << file << ":" << line << ") " << context.CurrentSuiteId << "." << _caseCnt << " " << message << endl;
            }
        }

        context.OutLines.push_back(sstr.str());
    }
    
    /// Record a test result line into the report.
    void RecordProperty(TestContext& context, const string& name, const string& value)
    {
        ostringstream pstr;
        if (context.Format == 'x')
        {
            pstr << "<property name=\"" << name << "\" value=\"" << value << "\" />";
            context.PropLines.push_back(pstr.str());
        }
        else // readable
        {
            pstr << "Property " << name << ":" << value << endl;
            context.OutLines.push_back(pstr.str());
        }
    }

    /// Record a verbatim text line into the report.
    void RecordVerbatim(TestContext& context, const string& message)
    {
        context.OutLines.push_back(message);
    }

    /// All test suite specifications must supply this.
    virtual void Run(TestContext&) = 0;

    /// Compliance.
    virtual ~TestSuite() {}
};

/// The orchestrator of the test execution.
class TestManager
{
private:
    /// List of pointers to suites.
    vector<TestSuite*> _suites;

    /// Present context.
    TestContext _context;

    /// Constructor.
    TestManager()
    {
        _context.Reset();
    }

    /// Destructor.
    ~TestManager() { }

public:
    /// Support for singleton pattern.
    static TestManager& Instance()
    {
        static TestManager self;
        return self;
    }
    
    /// Add a test suite to the run. All tests are statically allocated, so we don't need to destroy them manually.
    void AddSuite(TestSuite* ptc)
    {
        _suites.push_back(ptc);
    }

    /// Run all selected test suites.
    void RunSuites(vector<string> which, char fmt)
    {
        _context.Reset();
        _context.Format = fmt;

        int caseCnt = 0;
        int failCnt = 0;

        time_t tStart = time(nullptr);

        // Run through to execute suites.
        for (vector<TestSuite*>::iterator iter = _suites.begin(); iter != _suites.end(); ++iter)
        {
            // Is this suite requested?
            bool newSuite = (which.size() == 0);
            if(!newSuite)
            {
                for (vector<string>::iterator itRun = which.begin(); itRun != which.end(); ++itRun)
                {
                    if ((*iter)->ID().find(*itRun) != string::npos)
                    {
                        newSuite = true;
                    }
                }
            }

            if(newSuite)
            {
                /// New suite. Reset states.
                _context.CurrentSuiteId = (*iter)->ID().c_str();
                _context.PropLines.clear();

                /// Document the start of the suite.
                if(_context.Format == 'x')
                {
                    ostringstream oss;
                    oss << "    <testsuite name=" << (*iter)->ID() << ">" << endl;
                    (*iter)->RecordVerbatim(_context, oss.str());
                }
                else // readable
                {
                    ostringstream oss;
                    oss << endl << "Test Suite " << (*iter)->ID() << ": " << (*iter)->Description() << endl;
                    (*iter)->RecordVerbatim(_context, oss.str());
                }

                /// Run the suite.
                (*iter)->Run(_context);

                /// Completed the suite, update the counts.
                failCnt += (*iter)->FailCnt();
                caseCnt += (*iter)->CaseCnt();

                if(_context.Format == 'x')
                {
                    // Any properties?
                    if (_context.PropLines.size() > 0)
                    {
                        ostringstream oss1;
                        oss1 << "        <properties>" << endl;
                        (*iter)->RecordVerbatim(_context, oss1.str());

                        for (vector<string>::iterator piter = _context.PropLines.begin(); piter != _context.PropLines.end(); ++piter)
                        {
                            ostringstream ossp;
                            ossp << "            " << *piter << endl;
                            (*iter)->RecordVerbatim(_context, ossp.str());
                        }

                        ostringstream oss2;
                        oss2 << "        </properties>" << endl;
                        (*iter)->RecordVerbatim(_context, oss2.str());
                    }

                    ostringstream oss;
                    oss << "    </testsuite>" << endl;
                    (*iter)->RecordVerbatim(_context, oss.str());
                }
            }
        }

        /// Finished the test run, prepare the summary.
        time_t tEnd = time(nullptr);
        char sStartTime[32];
        strftime(sStartTime, 32, "%Y-%m-%d %H:%M:%S", localtime(&tStart));
        long dur = static_cast<long>(tEnd - tStart);

        if(_context.Format == 'x')
        {
            cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
            cout << "<testsuites" << " tests=" << caseCnt << " failures=" << failCnt << " time=" << dur << ">" << endl;
        }
        else // readable
        {
            cout << "#------------------------------------------------------------------" << endl;
            cout << "# Unit Test Report" << endl;
            cout << "# Start Time: " << sStartTime << endl;
            cout << "# Duration: " << dur << endl;
            cout << "# Cases Run: " << caseCnt << endl;
            cout << "# Cases Failed: " << failCnt << endl;
            cout << "# Test Result: " << ((failCnt > 0) ? "Fail" : "Pass") << endl;
            cout << "#--------------------------------------------------------------------" << endl;
        }

        /// Write out the test result lines.
        for (vector<string>::iterator iter = _context.OutLines.begin(); iter != _context.OutLines.end(); ++iter)
        {
            cout << *iter;
        }

        if(_context.Format == 'x')
        {
            cout << "</testsuites>" << endl;
        }
    }
};


/// Macro to define a single test suite.
#define UT_SUITE(id, desc)\
static class id##_TestSuite : public TestSuite\
{\
public:\
    id##_TestSuite() : TestSuite(#id, desc)\
    {\
        TestManager::Instance().AddSuite(this);\
    }\
    void Run(TestContext&);\
    ~id##_TestSuite(){} \
}id##_Instance;\
void id##_TestSuite::Run(TestContext& testContext)

/// Common code.
#define PASS_COMMON \
{\
    ostringstream oss;\
    RecordResult(testContext, true,  __FILE__, __LINE__, "");\
}

/// Prints the property name and value.
#define UT_PROPERTY(name, value)\
{\
    ostringstream oss;\
    oss << value;\
    RecordProperty(testContext, name, oss.str());\
}\

/// Prints the message with extra info.
#define UT_INFO(message, info)\
{\
if(testContext.Format == 'r')\
    {\
        ostringstream oss; \
        oss << message << " " << info << endl; \
        RecordVerbatim(testContext, oss.str()); \
    }\
}\

/// Checks whether the given condition is true.
#define UT_TRUE(condition)\
{\
    if (!(condition))\
    {\
        ostringstream oss;\
        oss << "[" << #condition << "] should be true";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the given condition is false.
#define UT_FALSE(condition)\
{\
    if (condition)\
    {\
        ostringstream oss;\
        oss << "[" << #condition << "] should be false";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is null.
#define UT_NULL(value1)\
{\
    if ((value1) != NULL)\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should be null";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is not null.
#define UT_NOT_NULL(value1)\
{\
    if ((value1) == NULL)\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should not be null";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is equal to the second.
#define UT_EQUAL(value1, value2)\
{\
    if ((value1) != (value2))\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should equal " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is not equal to the second.
#define UT_NOT_EQUAL(value1, value2)\
{\
    if ((value1) == (value2))\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should not equal " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is less than the second.
#define UT_LESS(value1, value2)\
{\
    if ((value1) >= (value2))\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should be less than " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is less than or equal to the second.
#define UT_LESS_OR_EQUAL(value1, value2)\
{\
    if ((value1) > (value2))\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should be less than or equal to " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is greater than the second.
#define UT_GREATER(value1, value2)\
{\
    if ((value1) <= (value2))\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should be greater than " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is greater than or equal to the second.
#define UT_GREATER_OR_EQUAL(value1, value2)\
{\
    if ((value1) < (value2))\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should be greater than or equal to " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first argument is within the given tolerance from
/// the second argument.  This is useful for comparing floating point values.
#define UT_CLOSE(value1, value2, tolerance)\
{\
    if (abs((value1)-(value2)) > tolerance)\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should be within " << tolerance << " of " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first string is equal to the second.
#define UT_STR_EQUAL(value1, value2)\
{\
    if (strcmp(value1, value2) != 0)\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should equal " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the first string is not equal to the second.
#define UT_STR_NOT_EQUAL(value1, value2)\
{\
    if (strcmp(value1, value2) == 0)\
    {\
        ostringstream oss;\
        oss << "[" << value1 << "] should not equal " << "[" << value2 << "]";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

/// Checks whether the string is empty, otherwise prints the contents.
#define UT_STR_EMPTY(value)\
{\
    if (strlen(value) > 0)\
    {\
        ostringstream oss;\
        oss << "[" << value << "] should be empty";\
        RecordResult(testContext, false,  __FILE__, __LINE__, oss.str());\
    }\
    PASS_COMMON\
}

#endif
