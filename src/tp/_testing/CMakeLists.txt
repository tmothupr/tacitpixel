
set(tacit_link_libs tacit)

option(BUILD_TESTS "Build Tests" OFF)
if(BUILD_TESTS)

	macro(testprg number)
		add_executable("test_${number}" "test_${number}.cxx")
		target_link_libraries("test_${number}" ${tacit_link_libs})
	endmacro()



	testprg(000)
	testprg(001)
	testprg(002)
	testprg(003)
	testprg(004)
	testprg(005)
	testprg(006)
	testprg(007)
	testprg(008)
	testprg(009)
	testprg(010)
	testprg(011)
	testprg(012)
	testprg(013)
	testprg(014)
	testprg(015)
	testprg(016)
	testprg(017)
	testprg(018)
	testprg(019)
	testprg(020)

	# others
	testprg(perf)
endif()