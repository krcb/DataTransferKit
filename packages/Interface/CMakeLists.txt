INCLUDE(CombinedOption)
INCLUDE(TribitsETISupport)

TRIBITS_SUBPACKAGE(Interface)

#
# Useless because there is nothing to instantiate but required by Tribits
#
TRIBITS_ADD_EXPLICIT_INSTANTIATION_OPTION()

TRIBITS_ADD_ETI_SUPPORT()

ASSERT_DEFINED(${PACKAGE_NAME}_ENABLE_EXPLICIT_INSTANTIATION)

TRIBITS_ADD_OPTION_AND_DEFINE(
  DataTransferKit_ENABLE_EXPLICIT_INSTANTIATION
  HAVE_DATATRANSFERKIT_EXPLICIT_INSTANTIATION
  "Enable explicit template instantiation (ETI) in DTK"
  ${${PACKAGE_NAME}_ENABLE_EXPLICIT_INSTANTIATION}
  )


#
# Add libraries, tests, and examples
#

ADD_SUBDIRECTORY(src)

TRIBITS_ADD_TEST_DIRECTORIES(test)

#TRIBITS_ADD_EXAMPLE_DIRECTORIES(example)

TRIBITS_SUBPACKAGE_POSTPROCESS()
