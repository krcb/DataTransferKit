//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   core/Coupler_Data_Field.hpp
 * \author Stuart Slattery
 * \date   Fri Nov 18 11:57:58 2011
 * \brief  Coupler_Data_Field class definition.
 */
//---------------------------------------------------------------------------//

#ifndef core_Coupler_Data_Field_hpp
#define core_Coupler_Data_Field_hpp

#include <string>

#include <Coupler_Data_Source.hpp>
#include <Coupler_Data_Target.hpp>

#include "Teuchos_RCP.hpp"

#include "Tpetra_Map.hpp"
#include "Tpetra_Export.hpp"

namespace coupler
{

//===========================================================================//
/*!
 * \class Data_Field
 * \brief Field type for data transfers. This exists for more a explicit
 * definition of fields in the coupler user interface.
 *
 * The Coupler_Data_Field encapsulates the relationship between a
 * Coupler_Data_Source and a Coupler_Data_Target. In addition to containing
 * the map for transfer between the source and the target, the field also
 * contains indicators for the status of the mapping (transfer cannot occur
 * unless a field has been mapped) and for whether a field is distributed
 * (requiring a parallel mapping) or scalar.
 */
/*! 
 * \example core/test/tstData_Field.cc
 *
 * Test of Coupler_Data_Field.
 */
//===========================================================================//

template<class DataType_T, class HandleType_T, class CoordinateType_T>
class Data_Field
{
  public:

    //@{
    //! Useful typedefs.
    typedef DataType_T                               DataType;
    typedef HandleType_T                             HandleType;
    typedef CoordinateType_T                         CoordinateType;
    typedef int                                      OrdinalType;
    typedef Data_Source<DataType,HandleType,CoordinateType> 
                                                     Data_Source_t;
    typedef Teuchos::RCP<Data_Source_t>              RCP_Data_Source;
    typedef Data_Target<DataType,HandleType,CoordinateType> 
                                                     Data_Target_t;
    typedef Teuchos::RCP<Data_Target_t>              RCP_Data_Target;
    typedef Tpetra::Map<OrdinalType>                 Tpetra_Map_t;
    typedef Teuchos::RCP<const Tpetra_Map_t>         RCP_Tpetra_Map;
    typedef Tpetra::Export<HandleType>               Tpetra_Export_t;
    typedef Teuchos::RCP<Tpetra_Export_t>            RCP_Tpetra_Export;
    //@}

  private:

    // Field name.
    std::string d_field_name;

    // Data transfer source implemenation.
    RCP_Data_Source d_source;

    // Data transfer target implemenation.
    RCP_Data_Target d_target;

    // Tpetra map for the data source.
    RCP_Tpetra_Map d_source_map;

    // Tpetra map for the data target.
    RCP_Tpetra_Map d_target_map;

    // Tpetra export for transfer from data source to data target.
    RCP_Tpetra_Export d_export;

    // Boolean for scalar field.
    bool d_scalar;

    // Boolean for field mapping. True if mapping complete.
    bool d_mapped;

  public:

    // Constructor.
    Data_Field(const std::string &field_name,
			RCP_Data_Source source,
			RCP_Data_Target target,
			bool scalar = false);

    // Destructor.
    ~Data_Field();

    //! Get the field name. 
    const std::string& name() 
    { return d_field_name; }

    //! Get the transfer data source.
    RCP_Data_Source source() 
    { return d_source; }

    //! Get the transfer data target.
    RCP_Data_Target target() 
    { return d_target; }
    
    //! Set the mapping for transfer from the data source to the data target.
    void set_mapping(RCP_Tpetra_Map source_map, RCP_Tpetra_Map target_map);

    //! Get the map for the data source.
    RCP_Tpetra_Map get_source_map()
    { return d_source_map; }

    //! Get the map for the data target.
    RCP_Tpetra_Map get_target_map()
    { return d_target_map; }

    //! Get the Tpetra export for transfer from data source to data target.
    RCP_Tpetra_Export get_export()
    { return d_export; }

    //! Return the scalar boolean.
    bool is_scalar()
    { return d_scalar; }

    //! Return the mapped boolean.
    bool is_mapped()
    { return d_mapped; }
};

} // end namespace coupler

//---------------------------------------------------------------------------//
// TEMPLATE MEMBERS
//---------------------------------------------------------------------------//

#include "Coupler_Data_Field.t.hpp"

#endif // core_Coupler_Data_Field_hpp

//---------------------------------------------------------------------------//
//              end of core/Coupler_Data_Field.hpp
//---------------------------------------------------------------------------//