//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   core/Coupler_DataField.hpp
 * \author Stuart Slattery
 * \date   Fri Nov 18 11:57:58 2011
 * \brief  Coupler_DataField class definition.
 */
//---------------------------------------------------------------------------//

#ifndef core_Coupler_DataField_hpp
#define core_Coupler_DataField_hpp

#include <string>

#include <Mesh_Point.hpp>

#include <Coupler_DataSource.hpp>
#include <Coupler_DataTarget.hpp>

#include "Teuchos_RCP.hpp"
#include "Teuchos_Comm.hpp"

#include "Tpetra_Map.hpp"
#include "Tpetra_Export.hpp"

namespace Coupler
{

//===========================================================================//
/*!
 * \class DataField
 * \brief Field type for data transfers. This exists for more a explicit
 * definition of fields in the coupler user interface.
 *
 * The Coupler_DataField encapsulates the relationship between a
 * Coupler_DataSource and a Coupler_DataTarget. In addition to containing
 * the map for transfer between the source and the target, the field also
 * contains indicators for the status of the mapping (transfer cannot occur
 * unless a field has been mapped) and for whether a field is distributed
 * (requiring a parallel mapping) or scalar.
 */
/*! 
 * \example core/test/tstDataField.cc
 *
 * Test of Coupler_DataField.
 */
//===========================================================================//

template<class DataType_T, class HandleType_T, class CoordinateType_T>
class DataField
{
  public:

    //@{
    //! Useful typedefs.
    typedef DataType_T                                       DataType;
    typedef HandleType_T                                     HandleType;
    typedef CoordinateType_T                                 CoordinateType;
    typedef int                                              OrdinalType;
    typedef Point<HandleType,CoordinateType>                 PointType;
    typedef DataSource<DataType,HandleType,CoordinateType>  DataSource_t;
    typedef Teuchos::RCP<DataSource_t>                      RCP_DataSource;
    typedef DataTarget<DataType,HandleType,CoordinateType>  DataTarget_t;
    typedef Teuchos::RCP<DataTarget_t>                      RCP_DataTarget;
    typedef Tpetra::Map<OrdinalType>                         Tpetra_Map_t;
    typedef Teuchos::RCP<const Tpetra_Map_t>                 RCP_Tpetra_Map;
    typedef Tpetra::Export<HandleType>                       Tpetra_Export_t;
    typedef Teuchos::RCP<Tpetra_Export_t>                    RCP_Tpetra_Export;
    typedef Teuchos::Comm<OrdinalType>                       Communicator_t;
    typedef Teuchos::RCP<const Communicator_t>               RCP_Communicator;
    //@}

  private:

    // Global communicator.
    RCP_Communicator d_comm;

    // Source field name.
    std::string d_source_field_name;

    // Target field name.
    std::string d_target_field_name;

    // Data transfer source implemenation.
    RCP_DataSource d_source;

    // Data transfer target implemenation.
    RCP_DataTarget d_target;

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
    DataField(RCP_Communicator comm_global,
	      const std::string &source_field_name,
	      const std::string &target_field_name,
	      RCP_DataSource source,
	      RCP_DataTarget target,
	      bool scalar = false);

    // Destructor.
    ~DataField();

    // Transfer data from the data source to the data target.
    void transfer();

    //! Get the communicator.
    RCP_Communicator comm() const
    { return d_comm; }

    //! Get the source field name. 
    const std::string& source_name() const
    { return d_source_field_name; }

    //! Get the transfer data source.
    RCP_DataSource source() 
    { return d_source; }

    //! Get the target field name. 
    const std::string& target_name() const
    { return d_target_field_name; }

    //! Get the transfer data target.
    RCP_DataTarget target() 
    { return d_target; }
    
    //! Return the scalar boolean.
    bool is_scalar() const
    { return d_scalar; }

    //! Return the mapped boolean.
    bool is_mapped() const
    { return d_mapped; }

    //! Return a const RCP to the source map.
    const RCP_Tpetra_Map source_map() const
    { return d_source_map; }

    //! Return a const RCP to the target map.
    const RCP_Tpetra_Map target_map() const
    { return d_target_map; }

  private:

    // Set the mapping for transfer from the data source to the data target
    // based on point mapping.
    void point_map();

    // Perform scalar transfer.
    void scalar_transfer();

    // Perform distributed transfer.
    void distributed_transfer();
};

} // end namespace Coupler

//---------------------------------------------------------------------------//
// TEMPLATE MEMBERS
//---------------------------------------------------------------------------//

#include "Coupler_DataField.t.hpp"

#endif // core_Coupler_DataField_hpp

//---------------------------------------------------------------------------//
//              end of core/Coupler_DataField.hpp
//---------------------------------------------------------------------------//