#pragma once
#ifndef __NM_API_HEADER__
#define __NM_API_HEADER__

#ifndef MAC_ADDRESS_SIZE
#define MAC_ADDRESS_SIZE 6
#endif

#ifndef NM_STATUS_API_VERSION_MISMATCHED
#define NM_STATUS_API_VERSION_MISMATCHED 0xE111000CL
#endif

const USHORT NMAPI_MAC_ADDRESS_SIZE = MAC_ADDRESS_SIZE;
const USHORT NMAPI_GUID_SIZE = 16;

///
/// <summary><c>NmCaptureFileMode</c>Netmon capture file expansion mode</summary> 
/// <remarks>
/// </remarks>
typedef enum _NmCaptureFileMode : UINT32
{
    NmCaptureFileWrapAround,
    NmCaptureFileChain,
    NmCaptureFileLastFlag

} NmCaptureFileMode;

///
/// <summary><c>NmCaptureCallbackExitMode</c>Capture callback function exit mode</summary> 
/// <remarks>
///     NmCaptureStopAndDiscard - NmStopCapture/NmPauseCapture returns immediately user's capture callback function will not be called after
///                               NmStopCapture/NmPauseCapture returns
/// </remarks>
typedef enum _NmCaptureCallbackExitMode : UINT32
{
    NmDiscardRemainFrames = 1,
    NmReturnRemainFrames = 2,

} NmCaptureCallbackExitMode;

///
/// <summary><c>NmAdapterOpState</c>Netmon driver adapter operational states</summary> 
/// <remarks>
/// </remarks>
typedef enum _NmAdapterOpState
{
    NmAdapterStateNone,
    NmAdapterStateBound,
    NmAdapterStateStopped,
    NmAdapterStateCapturing,
    NmAdapterStatePaused,

} NmAdapterOpState;

///
/// <summary><c>NmAdapterCaptureMode</c>Netmon driver adapter capture mode</summary> 
/// <remarks>
/// </remarks>
typedef enum _NmAdapterCaptureMode
{
    NmLocalOnly,
    NmPromiscuous

} NmAdapterCaptureMode;

///
/// <summary><c>NM_FRAME_CALLBACK</c>Callback function definition for frame capturing</summary> 
/// <remarks>
/// The user function must use __stdcall calling convention.
/// </remarks>
typedef VOID (CALLBACK *NM_FRAME_CALLBACK)(HANDLE hCaptureEngine,
                                           ULONG  ulAdapterIndex,
                                           PVOID  pCallerContext,
                                           HANDLE hFrame);

///
/// <summary><c>NM_NPL_PARSER_CALLBACK</c>Callback function definition for parser compile, frame parsing process status</summary> 
/// <remarks>
/// The user function must use __stdcall calling convention.
/// </remarks>
typedef VOID (CALLBACK *NM_NPL_PARSER_CALLBACK)(PVOID pCallerContext, 
                                                ULONG dwStatusCode, 
                                                LPCWSTR lpDescription, 
                                                ULONG ulType);

///
/// <summary><c>NmCallbackMsgType</c>Status levels of the call back message</summary> 
/// <remarks>
/// </remarks>
typedef enum _NmCallbackMsgType
{
    NmApiCallBackMsgTypeNone,
    NmApiCallBackMsgTypeError,
    NmApiCallBackMsgTypeWarning,
    NmApiCallBackMsgTypeInformation,
    NmApiCallBackMsgTypeLast

} NmCallbackMsgType;

///
/// <summary><c>NmNplParserLoadingOption</c>NPL loading option</summary> 
/// <remarks>
/// By default the NmLoadNplOptionNone is used.  Only the user specified NPL path(s) are loaded.
/// If both NmAppendRegisteredNplSets and a NPL path are specified, the resulting NPL parser will include
/// Both and the specified NPL path(s) are prefixed.
/// </remarks>
typedef enum _NmNplParserLoadingOption
{
    NmLoadNplOptionNone,
    NmAppendRegisteredNplSets

} NmNplParserLoadingOption;

///
/// <summary><c>NmFrameParserOptimizeOption</c>Frame parser optimization options</summary> 
/// <remarks>
/// Options used when create frame parser.
/// </remarks>
typedef enum _NmFrameParserOptimizeOption
{
    ///
    /// Create frame parser without optimization according to the added fitler
    ///
    NmParserOptimizeNone = 0,
    ///
    /// Create frame parser optimized based on added filters, fields and properties
    ///
    NmParserOptimizeFull = 1,

    NmParserOptimizeLast

} NmFrameParserOptimizeOption;

///
/// <summary><c>NmFrameParsingOption</c>Frame parser parsing options</summary> 
/// <remarks>
/// Options used by NmParseFrame function.
/// </remarks>
typedef enum _NmFrameParsingOption : UINT32
{
    NmParsingOptionNone = 0,
    ///
    /// Provide full path name of the current field if specified
    ///
    NmFieldFullNameRequired = 1,
    ///
    /// Provide the name of the protocol that contains the current field if specified
    ///
    NmContainingProtocolNameRequired = 2,
    ///
    /// Provide data type name of the current field
    ///
    NmDataTypeNameRequired = 4,
    ///
    /// Use caller specified frame number
    ///
    NmUseFrameNumberParameter = 8,
    ///
    /// Provide the display string of the field
    ///
    NmFieldDisplayStringRequired = 16,
    ///
    /// Provide the frame conversation information
    ///
    NmFrameConversationInfoRequired = 32,

    NmParsingOptionLast

} NmFrameParsingOption;

///
/// <summary><c>FRAME_PARSING_OPTION_MAX</c>The maximum value NmFrameParsingOption can be</summary> 
/// <remarks>
/// If there are n flags defined, The NmParsingOptionLast is 2**n + 1.  If n = 5, the constant is 63.
/// </remarks>
const UINT32 FRAME_PARSING_OPTION_MAX = ((NmParsingOptionLast - 1) * 2 - 1);

///
/// <summary><c>NmConversationOption</c>Frame parser conversation configuration options</summary> 
/// <remarks>
/// </remarks>
typedef enum _NmConversationConfigOption : UINT32
{
    NmConversationOptionNone,
    NmConversationOptionLast

} NmConversationOption;

///
/// <summary><c>NmReassemblyOption</c>Frame parser reassembly configuration options</summary> 
/// <remarks>
/// </remarks>
typedef enum _NmReassemblyConfigOption : UINT32
{
    NmReassemblyOptionNone,
    NmReassemblyOptionLast

} NmReassemblyOption;
                          
///
/// <summary><c>NmFrameFragmentationType</c>Fragmentation types returned in parsed frames</summary> 
/// <remarks>
/// </remarks>
typedef enum _NmFrameFragmentationType
{
    FragmentTypeNone,
    FragmentTypeStart,
    FragmentTypeMiddle,
    FragmentTypeEnd

} NmFrameFragmentationType;

/// <summary><c>NmParsedFieldProperty</c>The name string properties in parsed field</summary> 
/// <remarks>
/// </remarks>
typedef enum _NmParsedFieldNames
{
    NmFieldNamePath,
    NmFieldDataTypeName,
    NmFieldContainingProtocolName,
    NmFieldDisplayString

} NmParsedFieldNames;

///
/// <summary><c>NmMvsKeyType</c>Key types of the multi-value storage property</summary> 
/// <remarks>
/// The NmMvsKeyTypeNumber, NmMvsKeyTypeByteArray, NmMvsKeyTypeByteArray are the key types for retrieving the
/// Multi-value storage properties.
/// 
/// The NmMvsKeyTypeArrayIndex type is used as index value when retrieve property in the group property array.
/// </remarks>
typedef enum _NmMvsKeyType
{
    NmMvsKeyTypeNone,
    NmMvsKeyTypeNumber,
    NmMvsKeyTypeString,
    NmMvsKeyTypeByteArray,
    NmMvsKeyTypeArrayIndex,
    NmMvsKeyTypeLast

} NmMvsKeyType;

///
/// <summary>
/// <c>NmPropertyScope</c>
/// Scopes of properties.  It is reported in the property info.
/// </summary> 
/// <remarks>
/// </remarks>
///
typedef enum _NmPropertyScope
{
    NmPropertyScopeNone = 0,
    NmPropertyScopeConversation = 1,
    NmPropertyScopeGlobal = 2,
    NmPropertyScopeFrame = 4

} NmPropertyScope;

///
/// <summary>
/// <c>NmPropertyContainerType</c>
/// The property aggregation form.  
/// The regular form is a single value of NmPropertyValueType that can be addressed by just property name.
/// The multi-value storage is a set of properties that share the same name but different keys.
/// The array is a set of properties that stored in array and retrieved by name and index.
/// An array property is in the regular form if there is only one element.  The NmGetPropertyInfo may return
/// Container type NmPropertyContainerTypeValue if the key is not added along in NmAddProperty.
/// </summary> 
/// <remarks>
/// </remarks>
///
typedef enum _NmPropertyContainerType
{
    NmPropertyContainerTypeNone = 0,
    NmPropertyContainerTypeValue,
    NmPropertyContainerTypeMvs,
    NmPropertyContainerTypeArray

} NmPropertyContainerType;

///
/// <summary>
/// <c>NmPropertyValueType</c>
/// Type of the property value.
/// </summary> 
/// <remarks>
/// Number value is in signed or unsigned integer format
/// String value is in wide char format
/// Byte Blob is in byte array
///
/// The value type of properties, in the same multi value storage property addressed 
/// By different keys or in the same property group by different indexes,
/// Can be different.
/// </remarks>
///
typedef enum _NmPropertyValueType
{
    NmPropertyValueNone,
    NmPropertyValueSignedNumber,
    NmPropertyValueUnsignedNumber,
    NmPropertyValueString,
    NmPropertyValueByteBlob

} NmPropertyValueType, *PNmPropertyValueType;

///
/// <summary><c>NM_PROPERTY_STORAGE_KEY</c></summary> 
/// <remarks>
/// NM_PROPERTY_STORAGE_KEY is used with property name or id obtained from NmAddProperty.
/// KeyNumber, KeyString and KeyBuffer are for Multi-Value stroage.  The KeyLength is required for KeyBuffer.
/// ArrayIndex is for specifying the array index of the property group specified by NmMvsKeyTypeArrayIndex.
/// </remarks>
typedef struct _NM_PROPERTY_STORAGE_KEY
{
    USHORT          Size;
    ULONG           KeyLength;
    NmMvsKeyType    KeyType;
    union
    {
        /// The numeric key for the property in the multi value storage
        UINT64      KeyNumber;
        /// The wide string key for the property in the multi value storage.  It must be null terminated.
        LPWSTR      KeyString;
        /// The key in the form of byte array for the property in the multi value storage
        PBYTE       KeyBuffer;
        /// The index for the property in the property group
        UINT64      ArrayIndex;
    } Key;

} NM_PROPERTY_STORAGE_KEY, *PNM_PROPERTY_STORAGE_KEY;

///
/// Contains runtime information for instantiated properties
///
typedef struct _NM_PROPERTY_INFO
{
    /// For version control
    USHORT Size;
    /// Property Scope
    NmPropertyScope Scope;
    /// Property container type, e.g., MVS, Array.
    NmPropertyContainerType ContainerType;
    /// The element count of the name excluding the terminator.
    /// When no name buffer is provided, i.e., Name parameter is NULL, NameSize is only used as output
    /// Parameter that returns the actual name length.
    /// When the name buffer is provided, the NameSize are both input and output parameters.
    /// The NameSize acts as a input parameter specifying the buffer length.  The name's actual length
    /// Is also returned by NmGetPropertyInfo.
    USHORT NameSize;
    /// Property string added by NmAddProperty.  If a buffer is assigned when call NmGetPropertyInfo, 
    /// The NameSize must specify the buffer length (in element count.)  NmGetPropertyInfo returns the 
    /// Property Name in the buffer if the buffer has enough space.  Or buffer over flow error is returned.
    LPWSTR Name;
    /// The data type of the property.  If the value type is string, the terminator is not included.
    NmPropertyValueType ValueType;
    /// The size of the value in BYTE.  If the value type is string, the terminator is excluded.
    ULONG ValueSize;
    /// number of items in Array.  The regular an multi-value storage properties have only one item
    ULONG ItemCount;

} NM_PROPERTY_INFO, *PNM_PROPERTY_INFO;

/// <summary><c>NM_FRAGMENTATION_INFO</c>Fragmentation information returned in parsed frames</summary> 
/// <remarks>
/// </remarks>
typedef struct _NM_FRAGMENTATION_INFO
{
    USHORT  Size;
    WCHAR   FragmentedProtocolName[MAX_PATH];
    WCHAR   PayloadProtocolName[MAX_PATH];
    NmFrameFragmentationType FragmentType;

} NM_FRAGMENTATION_INFO, *PNM_FRAGMENTATION_INFO;

///
/// <summary><c>NM_NIC_ADAPTER_INFO</c> Contain adapter information.</summary> 
/// <remarks>
/// </remarks>
///
typedef struct  _NM_NIC_ADAPTER_INFO
{
    USHORT                  Size;
    UCHAR                   PermanentAddr[MAC_ADDRESS_SIZE];
    UCHAR                   CurrentAddr[MAC_ADDRESS_SIZE];
    NDIS_MEDIUM             MediumType;
    NDIS_PHYSICAL_MEDIUM    PhysicalMediumType;
    WCHAR                   ConnectionName[MAX_PATH];
    WCHAR                   FriendlyName[MAX_PATH];
    WCHAR                   Guid[MAX_PATH];

    ///
    /// Network adapter operational state. Indicates if the network adapter is bound, capturing, pause or stopped
    ///
    NmAdapterOpState        OpState;
    ///
    /// Indicates if the network adapter is enabled or disabled. It only can be enabled if it is bound to the netmon driver
    ///
    BOOL                    Enabled;
    BOOL                    PModeEnabled;

    ///
    /// Frame indication callback is assigned per adapter
    ///
    NM_FRAME_CALLBACK       CallBackFunction;

} NM_NIC_ADAPTER_INFO, *PNM_NIC_ADAPTER_INFO;

///
/// <summary><c>NM_API_CONFIGURATION</c>Contain all configurable API parameters</summary> 
/// <remarks>
/// </remarks>
///
typedef struct _NM_API_CONFIGURATION
{
    ///
    /// Caller sets the member 'Size' when pass in allocated buffer 
    ///
    USHORT  Size;

    ///
    /// Configurable limits that overwrite default API settings 
    ///
    ULONG RawFrameHandleCountLimit;
    ULONG ParsedFrameHandleCountLimit;
    ULONG CaptureEngineHandleCountLimit;
    ULONG NplParserHandleCountLimit;
    ULONG FrameParserConfigHandleCountLimit;
    ULONG FrameParserHandleCountLimit;
    ULONG CaptureFileHandleCountLimit;

    ///
    /// API threading mode for COM initialization.  Only support COINIT_MULTITHREADED and COINIT_APARTMENTTHREADED
    ///
    COINIT  ThreadingMode;

    ///
    /// Configurable default feature/behavior parameters
    ///
    NmConversationOption      ConversationOption;
    NmReassemblyOption        ReassemblyOption;
    NmCaptureFileMode         CaptureFileMode;
    NmFrameParsingOption      FrameParsingOption;
    NmCaptureCallbackExitMode CaptureCallbackExitMode;

    ///
    /// Hard limits the API enforce (not configurable)
    ///
    ULONG MaxCaptureFileSize;
    ULONG MinCaptureFileSize;
    
    /// Maximum number of handles per handle type  
    ULONG MaxApiHandleLimit;

} NM_API_CONFIGURATION, *PNM_API_CONFIGURATION;

///
/// <summary><c>NM_PROTOCOL_SEQUENCE_CONFIG</c>Data structure for API user to specify NPL properties and fields
///                                            For frame order correction support.
/// </summary> 
/// <remarks>
/// </remarks>
///
typedef struct _NM_PROTOCOL_SEQUENCE_CONFIG
{
    ///
    /// API verifies the member 'Size' against the size of its version.  They must match.
    ///
    USHORT Size;

    ///
    /// The names of the properties containing the values to form the key 
    /// to identify the group of the frames to get in order.  If multiple names are used,
    /// They are separated by semicolons.  The string must be NULL terminated.
    ///
    LPWSTR GroupKeyString;

    ///
    /// The name of the property containing the frame's sequence number.
    ///
    LPWSTR SequencePropertyString;

    ///
    /// The name of the property containing the frame's next sequence number.
    ///
    LPWSTR NextSequencePropertyString;

} NM_PROTOCOL_SEQUENCE_CONFIG, *PNM_PROTOCOL_SEQUENCE_CONFIG;

///
/// <summary><c>NM_OPEN_CAP_PARAMETER</c>Data structure for calling NmOpCaptureFileInOrder</summary>
/// 
/// <remarks>
/// </remarks>
///
typedef struct _NM_ORDER_PARSER_PARAMETER
{
    ///
    /// API verifies the member 'Size' against the size of its version.  They must match.
    ///
    USHORT Size;

    ///
    /// The frame parser used for handling out of order frames.  It must be built from a frame parser
    /// Configuration that has sequence information specified by NM_PROTOCOL_SEQUENCE_CONFIG.
    ///
    HANDLE FrameParser;

    ///
    /// For future option flags.
    ///
    ULONG Option;

} NM_ORDER_PARSER_PARAMETER, *PNM_ORDER_PARSER_PARAMETER;

///
/// <summary><c>NM_PARSED_DATA_FIELD</c> Return structure contains the parsed field.</summary> 
/// <remarks>
/// Returned to caller from NmGetParsedFieldInfo function
/// </remarks>
///
typedef struct _NM_PARSED_FIELD_INFO
{
    ///
    /// API verifies the member 'Size' against the size of its version.  They must match.
    ///
    USHORT   Size;
    USHORT  FieldIndent;
    ///
    /// The size of the string that holds the full path of the data field if the NmFrameParseOptions 
    /// NmFieldFullNameRequired is set, e.g., Frame.Ethernet.IPv4.SourceAddress;  Otherwise it is the size
    /// of the current field name only
    ///
    USHORT  NamePathLength;
    ///
    /// The size of the string that contains the name of the NPL data type if the NmFrameParseOptions 
    /// NmDataTypeNameRequired is set, e.g., L"UINT16";  Otherwise it is zero.
    ///
    USHORT  NplDataTypeNameLength;
    ///
    /// The size of the string that contains the protocol containing the field if the NmFrameParseOptions 
    /// NmContainingProtocolNameRequired is set;  Otherwise it is zero
    ///
    USHORT  ProtocolNameLength;
    ///
    /// The size of the display string of the field if the NmFrameParseOptions 
    /// NmFieldDisplayStringRequired is set;  Otherwise it is zero
    ///
    USHORT  DisplayStringLength;
    ///
    /// Offset in current protocol
    ///
    ULONG   ProtocolBitOffset;
    ///
    /// Field offset in frame
    ///
    ULONG   FrameBitOffset;
    ///
    /// Length of the field in bits
    ///
    ULONG   FieldBitLength;
    ///
    /// The variant type defined as in VARENUM
    ///
    USHORT  ValueType;
    ///
    /// The size of the buffer required to hold the field value represented in VARIANT struct including
    /// The length of the content if the VARIANT contains a pointer to ARRAY or string.
    ///
    USHORT  ValueBufferLength;

} NM_PARSED_FIELD_INFO, *PNM_PARSED_FIELD_INFO;

//////////////////////////////////////////////
///
///NmApi function declarations
///
//////////////////////////////////////////////
/// <summary><c>NmGetApiVersion</c>Query current version</summary> 
/// <remarks>
/// The API version matches netmon engine version.
/// </remarks>
/// <example> This sample shows how to call the NmGetApiVersion method.
/// <code>
///     USHORT majorNumber = 0;
///     USHORT minorNumber = 0;
///     USHORT BuildNumber = 0;
///     USHORT RevisionNumber = 0;
///     NmGetApiVersion(&majorNumber, &minorNumber, &BuildNumber, &RevisionNumber);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="Major">[out] Major version number</param>
/// <param name="Minor">[out] Minor version number</param>
/// <param name="Build">[out] Build number</param>
/// <param name="Revision">[out] Revision number</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>Return nothing</returns>
extern "C" VOID WINAPI NmGetApiVersion(__out PUSHORT Major, __out PUSHORT Minor, __out PUSHORT Build, __out PUSHORT Revision);

/// <summary><c>NmGetApiConfiguration</c>Return current API configuration parameters</summary> 
/// <remarks>
/// Caller provides the non-null pointer of the NM_API_CONFIGURATION struct. 
/// </remarks>
/// <example> This sample shows how to call the NmGetApiConfiguration method.
/// <code>
///     NM_API_CONFIGURATION apiConfiguration;
///     apiConfiguration.Size = sizeof(NM_API_CONFIGURATION);
///     NmGetApiConfiguration(&apiConfiguration);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="pConfiguration">[out] Struct pointer for API to fill</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     NM_API_VERSION_MISMATCHED: The version of NM_API_CONFIGURATION struct is different
///     ERROR_BAD_ARGUMENTS: pConfiguration is NULL.
/// </returns>
extern "C" ULONG WINAPI NmGetApiConfiguration(__in PNM_API_CONFIGURATION pConfiguration);

/// <summary><c>NmApiInitialize</c>Overwrite default configuration.</summary> 
/// <remarks>
/// Caller needs to provide storage for NM_API_CONFIGURATION struct.
/// </remarks>
/// <example> This sample shows how to call the NmApiInitialize method.
/// <code>
///     ULONG status = ERROR_SUCCESS;
///     NM_API_CONFIGURATION apiConfig;
///     apiConfig.Size = sizeof(NM_API_CONFIGURATION);
///     // specify all configuration parameters before call 
///     status = NmApiInitialize(&apiConfig);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="ApiConfig">[in] Caller specified API configuration parameter struct</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     NM_API_VERSION_MISMATCHED: The version of NM_API_CONFIGURATION struct is different
///     ERROR_INVALID_STATE: Can not change API configuration
/// </returns>
extern "C" ULONG WINAPI NmApiInitialize(__in PNM_API_CONFIGURATION ApiConfig);

/// <summary><c>NmApiClose</c>Release API resouces</summary> 
/// <remarks>
/// Should be called when done with the API
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
/// </returns>
extern "C" ULONG WINAPI NmApiClose();

/// <summary><c>NmOpenCaptureEngine</c>Open a capture engine</summary> 
/// <remarks>
/// 
/// </remarks>
/// <example> Description
/// <code>
///     
///     
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="phCaptureEngine">[out] The returned handle to the capture engine object on success</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle
///     ERROR_INVALID_STATE:     The operation is not available.
///     ERROR_NOT_ENOUGH_MEMORY: Fail to allocate memory for the object.
/// </returns>
extern "C" ULONG WINAPI NmOpenCaptureEngine(__out PHANDLE phCaptureEngine);

/// <summary><c>NmGetAdapterCount</c>Return number of the adapters that the capture engine can access</summary> 
/// <remarks>
/// 
/// </remarks>
/// <example> Description
/// <code>
///     
///     
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hCaptureEngine">[in] The capture engine under query</param>
/// <param name="pulCount">[out] The returned count of adapters</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found capture engine specified
/// </returns>
extern "C" ULONG WINAPI NmGetAdapterCount(__in HANDLE hCaptureEngine , __out PULONG pulCount);

/// <summary><c>NmGetAdapter</c>Get adapter information from the capture engine</summary> 
/// <remarks>
/// Caller can use name, GUID etc. to select adapter to use.  The adapter index should be within the 
/// Range returned by NmGetAdapterCount method.  Caller needs to provide the storage of the
/// NM_FRAME_CALLBACK struct.
/// </remarks>
/// <example> Description
/// <code>
///     
///     
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hCaptureEngine">[in] The handle of the capture engine object</param>
/// <param name="ulIndex">[in] The index number of the adapter to retrieve</param>
/// <param name="pNMAdapterInfo">[out] The returned adapter information struct</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: Either the capture engine or the adapter indicated by index is not found.
///     NM_API_VERSION_MISMATCHED: The version of NM_NIC_ADAPTER_INFO struct is different
/// </returns>
extern "C" ULONG WINAPI NmGetAdapter(__in HANDLE hCaptureEngine, __in ULONG ulIndex, __out PNM_NIC_ADAPTER_INFO pNMAdapterInfo);

/// <summary><c>NmConfigAdapter</c>Configure the adapter with the frame indication callback and the caller context.</summary> 
/// <remarks>
/// The current callback function and context will overwrite the previous ones.  The adapter index number
/// Must be in the range returned from NmGetAdapterCount method.
/// </remarks>
/// <example> Description
/// <code>
///     
///     
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hCaptureEngine">[in] The handle to the target capture engine</param>
/// <param name="ulIndex">[in] The index number of the target adapter</param>
/// <param name="CallbackFunction">[in] The frame indication callback function pointer to set</param>
/// <param name="pCallerContext">[in] The caller context pointer</param>
/// <param name="ExitMode">[in] The callback function exit mode</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle
///     ERROR_NOT_FOUND: Either the capture engine or the adapter is not found.
/// </returns>
extern "C" ULONG WINAPI NmConfigAdapter(__in HANDLE hCaptureEngine,
                                        __in ULONG ulIndex, 
                                        __in NM_FRAME_CALLBACK CallbackFunction, 
                                        __in PVOID pCallerContext, 
                                        __in NmCaptureCallbackExitMode ExitMode = NmDiscardRemainFrames);

/// <summary><c>NmStartCapture</c>Start capture on the specified capture engine and adapter</summary> 
/// <remarks>
/// Capture mode can be PMODE and LocalOnly.
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hCaptureEngine">[in] The handle to the target capture engine</param>
/// <param name="ulAdapterIndex">[in] The index number of the target adapter</param>
/// <param name="CaptureMode">[in] The capture mode, PMODE or LOCAL_ONLY</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle
///     ERROR_NOT_FOUND: not found capture engine or adapter specified
///     ERROR_INVALID_STATE: Can not pause at current state
/// </returns>
extern "C" ULONG WINAPI NmStartCapture(__in HANDLE hCaptureEngine, __in ULONG ulAdapterIndex, __in NmAdapterCaptureMode CaptureMode);

/// <summary><c>NmPauseCapture</c>Pause the capture</summary> 
/// <remarks>
/// 
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hCaptureEngine">[in] The handle to the target capture engine</param>
/// <param name="ulAdapterIndex">[in] The index number of the target adapter</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle
///     ERROR_INVALID_STATE: Can not pause at current state
///     ERROR_NOT_FOUND: not found capture engine or adapter specified
/// </returns>
extern "C" ULONG WINAPI NmPauseCapture(__in HANDLE hCaptureEngine, __in ULONG ulAdapterIndex);

/// <summary><c>NmResumeCapture</c>Resume the capture that is previously paused</summary> 
/// <remarks>
/// Cannot resume after NmStopCapture is called.  The frame indication callback is no longer invoked 
/// Until NmResumeCapture method is called
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hCaptureEngine">[in] The handle to the target capture engine</param>
/// <param name="ulAdapterIndex">[in] The index number of the target adapter</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle
///     ERROR_INVALID_STATE: Can not resume at current state
///     ERROR_NOT_FOUND: not found capture engine or adapter specified
/// </returns>
extern "C" ULONG WINAPI NmResumeCapture(__in HANDLE hCaptureEngine, __in ULONG ulAdapterIndex);

/// <summary><c>NmStopCapture</c>Stop capture on given capture engine and adapter</summary> 
/// <remarks>
/// The frame indication callback is no longer invoked.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hCaptureEngine">[in] The handle to the target capture engine</param>
/// <param name="ulAdapterIndex">[in] The index number of the target adapter</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle
///     ERROR_INVALID_STATE: Can not stop at current state
///     ERROR_NOT_FOUND: not found capture engine or adapter specified
/// </returns>
extern "C" ULONG WINAPI NmStopCapture(__in HANDLE hCaptureEngine, __in ULONG ulAdapterIndex);

//////////////////////////////////////////////////////
/// Parsing functions
//////////////////////////////////////////////////////

/// <summary><c>NmLoadNplParser</c>Load NPL scripts and create NPL parser</summary> 
/// <remarks>
/// Callback function is invoked for compile error/warning/info.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="pFileName">[in] The start parser script file name</param>
/// <param name="ulFlags">[in] Option flags</param>
/// <param name="CallbackFunction">[in] The parser compiler error callback function pointer</param>
/// <param name="pCallerContext">[in] The caller context pointer that will be passed back to the callback function</param>
/// <param name="phNplParser">[Out] The returned handle to the NPL parser object</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_ENOUGH_MEMORY: Fail to create NPL parser object
/// </returns>
extern "C" ULONG WINAPI NmLoadNplParser(__in LPCWSTR pFileName,
                                        __in NmNplParserLoadingOption ulFlags,
                                        __in NM_NPL_PARSER_CALLBACK CallbackFunction,
                                        __in PVOID pCallerContext,
                                        __out PHANDLE phNplParser);

/// <summary><c>NmCreateFrameParserConfiguration</c>Create frame parser configuration that contains the filter and field configuration</summary> 
/// <remarks>
/// All the frame parser features including conversation and reassembly must be added in the configuration before creating the frame parser.
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hNplParser">[in] The handle of the NPL parser used for the frame parser</param>
/// <param name="CallbackFunction">[in] The compiler error callback function pointer</param>
/// <param name="pCallerContext">[in] The caller context pointer that will be passed back to the callback function</param>
/// <param name="phFrameParserConfiguration">[out] The returned handle of the frame parser configuration object</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_ENOUGH_MEMORY: Fail to create frame parser configuration object.
///     ERROR_NOT_FOUND: not found specified NPL parser
///     
/// </returns>
extern "C" ULONG WINAPI NmCreateFrameParserConfiguration(__in HANDLE hNplParser,
                                                         __in NM_NPL_PARSER_CALLBACK CallbackFunction,
                                                         __in PVOID pCallerContext,
                                                         __out PHANDLE phFrameParserConfiguration);

/// <summary><c>NmAddFilter</c>Add filter for optimizing frame parser</summary> 
/// <remarks>
/// The filter id is used to evaluate the state of the filter on a parsed frame.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfiguration">[in] The handle of the target frame parser configuration object</param>
/// <param name="pFilterString">[in] The text of the filter</param>
/// <param name="pulFilterId">[out] The returned filter index in the frame parser</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser configuration
/// </returns>
extern "C" ULONG WINAPI NmAddFilter(__in HANDLE hFrameParserConfiguration, __in LPCWSTR pFilterString, __out PULONG pulFilterId);

/// <summary><c>NmAddField</c>Add field for optimizing frame parser</summary> 
/// <remarks>
/// All the fields are enumerated in the parsed frame if no field is added.  The field id is used to retrieve the field 
/// In the parsed frame.  Caller needs to provide unique fully qualified field name, e.g., TCP.Option.Ack.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfiguration">[in] The handle of the target frame parser configuration object</param>
/// <param name="pFilterString">[in] The text of the field</param>
/// <param name="pulFilterId">[out] The returned field index in the frame parser</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser configuration
/// </returns>
extern "C" ULONG WINAPI NmAddField(__in HANDLE hFrameParserConfiguration, __in LPCWSTR pFqfnString, __out PULONG pulFieldId);

/// <summary><c>NmAddProperty</c>Add a property to the configuration.</summary> 
/// <remarks>
/// The property name should have scope prefix such as Conversation, Global, etc.  If not specified, 
/// The frame property is the default scope.
/// The multi-value storage property name can contain the key, e.g., Property.myMvs$[key] as in NPL parer.  The key can be number, string or other property name.
/// The array property name can contain the index, e.g., Global.myArray[3] as in NPL parser.
/// </remarks>
/// <example> This sample shows how to call the NmAddProperty method.
/// <code>
///     HANDLE hFrameParserConfiguration;
///     ULONG myPropID;
///     NmAddProperty(hFrameParserConfiguration, L"Property.TCPPayloadLength", &myPropID);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfiguration">[in] Frame Parser Configuration Handle</param>
/// <param name="pPropertyString">[in] Fully qualified name of the property.</param>
/// <param name="pulPropertyId">[out] Returned ID used to reference the property.</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser configuration
///     ERROR_INVALID_PARAMETER: The specified property name is invalid
/// </returns>
extern "C" ULONG WINAPI NmAddProperty(__in HANDLE hFrameParserConfiguration, __in LPCWSTR pPropertyString, __out PULONG pulPropertyId);

/// <summary><c>NmAddSequenceOrderConfig</c>Add protocol sequence order configurations</summary> 
/// <remarks>
/// The protocol sequence configuration includes the properties that define the related protocols' sequence control data such
/// As TCP sequence number.
/// </remarks>
/// <example> Description
/// <code>
///     
///     
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfig">[in] Caller provided sequence configuration data</param>
/// <param name="pSeqConfig">[in] Caller provided sequence configuration data</param>
/// <param name="pulIndex">[out] The index of the added configuration in API</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: NULL pointer
///     ERROR_NOT_ENOUGH_MEMORY: Fail to allocate memory to store the configuration.
/// </returns>
extern "C" ULONG WINAPI NmAddSequenceOrderConfig(__in HANDLE hFrameParserConfig,
                                                 __in PNM_PROTOCOL_SEQUENCE_CONFIG pSeqConfig,
                                                 __out PULONG pulIndex);

/// <summary><c>NmConfigReassembly</c>Enable or disable reassembly</summary> 
/// <remarks>
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfiguration">[in] The handle of the target frame parser configuration object</param>
/// <param name="Option">[in] Reassembly options</param>
/// <param name="bEnable">[in] Action to take, enable or disable</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser configuration
/// </returns>
extern "C" ULONG WINAPI NmConfigReassembly(__in HANDLE hFrameParserConfiguration, __in NmReassemblyOption Option, __in BOOL bEnable);

/// <summary><c>NmConfigConversation</c>Configure conversation options</summary> 
/// <remarks>
/// </remarks>
/// <example> Description
/// <code>
///     
///     
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfiguration">[in] The handle of the target frame parser configuration object</param>
/// <param name="Option">[in] conversation configuration options</param>
/// <param name="bEnable">[in] Action to take, enable or disable</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
/// </returns>
extern "C" ULONG WINAPI NmConfigConversation(__in HANDLE hFrameParserConfiguration, __in NmConversationOption Option, __in BOOL bEnable);

/// <summary><c>NmConfigStartDataType</c>Configure start data type</summary> 
/// <remarks>
/// By default, frame parser start parsing a frame from Netmon built-in protocol "Frame".
/// This function let caller set the data type to start.  This is especially provided for parsing a arbitrary
/// Data buffer with the frame parser starting from the data type that is configured with this function.
/// </remarks>
/// <example> Description
/// <code>
///     
///     
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfiguration">[in] The handle of the target frame parser configuration object</param>
/// <param name="pStartDataTypeName">[in] The name of the data type that the created frame parser starts with</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser configuration
///     ERROR_INSUFFICIENT_BUFFER: The given start type name is longer than 260 characters.
/// </returns>
extern "C" ULONG WINAPI NmConfigStartDataType(__in HANDLE hFrameParserConfiguration, __in LPCWSTR pStartDataTypeName);

/// <summary><c>NmGetStartDataType</c>Return the start data type of the given frame parser configuration</summary> 
/// <remarks>
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfiguration">[in] The handle of the target frame parser configuration object</param>
/// <param name="ulBufferLength">[in] Caller buffer length</param>
/// <param name="pStartDataTypeName">[out] Caller buffer to hold the name of the data type that the created frame parser starts with</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser configuration
///     ERROR_INSUFFICIENT_BUFFER: The truncated name is in provided buffer
/// </returns>
extern "C" ULONG WINAPI NmGetStartDataType(__in HANDLE hFrameParserConfiguration,
                                           __in ULONG ulBufferELength,
                                           __out_ecount(ulBufferELength) LPWSTR pStartDataTypeName);

/// <summary><c>NmCreateFrameParser</c>Create frame parser from given configuration</summary> 
/// <remarks>
/// The optimization option is set to NmParserOptimizeNone by default that no optimization is applied.
/// The existing native applications do not need to recompile.  The new application can take advantage of this flag to 
/// Force optimization in the scenario where no field is added.  Without this option, the caller can only get a non-optimized 
/// Parser so that all the fields are included in the parsed frame.  With this option, an optimized frame parser can be
/// generated to serve the dedicated filtering scenarios.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParserConfiguration">[in] The handle of the source frame parser configuration object</param>
/// <param name="phFrameParser">[out] The returned handle of the frame parser</param>
/// <param name="OptimizeOption">[in] The optimization flag for creating the frame parser</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser configuration
/// </returns>
extern "C" ULONG WINAPI NmCreateFrameParser(__in HANDLE hFrameParserConfiguration,
                                            __out PHANDLE phFrameParser,
                                            __in NmFrameParserOptimizeOption OptimizeOption = NmParserOptimizeNone);

/// <summary><c>NmParseFrame</c>Parse the raw netmon frame and return it in parsed format</summary> 
/// <remarks>
/// The parsed frame contains the frame information, filter state and enumeration of fields.  When reassembly is
/// Enabled, the last fragment of the payload completing the reassembly process and insert the reassembled raw frame.
/// The ulFrameNumber parameter is for conversation or global properties if using frame number as the key.  If the same
/// Frame number is used for different frames, the properties's values may be overwritten by the last instance.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParser">[in] The handle of the frame parser used to parse the given raw frame object</param>
/// <param name="hRawFrame">[in] The handle of the target raw frame to parser</param>
/// <param name="ulFrameNumber">[in] The frame number should be used in parsing process if enabled by option flag</param>
/// <param name="ulOption">[in] See flag definition NmFrameParsingOption</param>
/// <param name="phParsedFrame">[out] The handle to the result parsed frame object</param>
/// <param name="InsertedRawFrameCount">[out] the handle of the inserted raw frame object as the result of parsing, e.g., reassembly</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser or raw frame
/// </returns>
extern "C" ULONG WINAPI NmParseFrame(__in HANDLE hFrameParser,
                                     __in HANDLE hRawFrame,
                                     __in ULONG ulFrameNumber,
                                     __in ULONG ulOption,
                                     __out PHANDLE phParsedFrame,
                                     __out PHANDLE phInsertedRawFrame);

/// <summary><c>NmParseBuffer</c>Parse the given data buffer and return it in parsed format</summary> 
/// <remarks>
/// The data buffer contains the byte array that can be a raw frame, part of raw frame or any arbitrary data.
/// The parsed frame contains the fabricated frame information. The filter state and enumeration of field are supported.
/// The inter frame reassembly is not supported since it requires multiple frames and conversation that are 
/// Not available in buffer mode.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParser">[in] The handle of the frame parser used to parse the Given frame object</param>
/// <param name="pDataBuffer">[in] The pointer to the target data buffer</param>
/// <param name="ulBufferLength">[in] length of the data buffer in previous parameter</param>
/// <param name="ulFrameNumber">[in] The frame number should be used in parsing process if enabled by option flag</param>
/// <param name="ulOption">[in] See flag definition NmFrameParsingOption</param>
/// <param name="phParsedFrame">[out] The handle to the result parsed frame object</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser or raw frame
/// </returns>
extern "C" ULONG WINAPI NmParseBuffer(__in HANDLE hFrameParser,
                                      __in_bcount(ulBufferLength) PBYTE pDataBuffer,
                                      __in ULONG ulBufferLength,
                                      __in ULONG ulFrameNumber,
                                      __in ULONG ulOption,
                                      __out PHANDLE phParsedFrame);

/// <summary><c>NmBuildRawFrameFromBuffer</c>Build a raw frame using a given data buffer</summary> 
/// <remarks>
/// The data buffer is transformed to a raw frame object.  The media type, time stamp are optional.  Their default
/// Values are zero.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="pDataBuffer">[in] The pointer to the target data buffer</param>
/// <param name="ulBufferLength">[in] length of the data buffer in previous parameter</param>
/// <param name="ulMedia">[in] Media type of the target raw frame</param>
/// <param name="ullTimeStamp">[in] Capture time stamp of the target raw frame</param>
/// <param name="phRawFrame">[out] The handle to the result parsed frame object</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_ENOUGH_MEMORY: No space to build the new frame
/// </returns>
extern "C" ULONG WINAPI NmBuildRawFrameFromBuffer(__in_bcount(ulBufferLength) PBYTE pDataBuffer,
                                                  __in ULONG ulBufferLength,
                                                  __in ULONG ulMedia,
                                                  __in UINT64 ullTimeStamp,
                                                  __out PHANDLE phRawFrame);

/// <summary><c>NmGetFrameFragmentInfo</c>Return fragment information of the given parsed frame</summary> 
/// <remarks>
/// Raw frame does not aware of its fragment type. Only parsing the frame can tell when reassembly is enabled.
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="pFragmentationInfo">[out] Caller provided struct pointer</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame
///     ERROR_INSUFFICIENT_BUFFER: If the protocol name length is longer than the buffer in PNmReassemblyInfo struct
/// </returns>
extern "C" ULONG WINAPI NmGetFrameFragmentInfo(__in HANDLE hParsedFrame, __out PNM_FRAGMENTATION_INFO pFragmentationInfo);

/// <summary><c>NmGetFilterCount</c>Return configured filter count in the given frame parser</summary> 
/// <remarks>
/// 
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParser">[in] frame parser under inspection</param>
/// <param name="pulFilterCount">[out] number of filters of the given frame parser.  It is zero if return code is not success</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser
/// </returns>
extern "C" ULONG WINAPI NmGetFilterCount(__in HANDLE hFrameParser, __out PULONG pulFilterCount);

/// <summary><c>NmEvaluateFilter</c>Return the state of specified filter in given parsed frame</summary> 
/// <remarks>
/// 
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame to evaluate</param>
/// <param name="ulFilterId">[in] The identify number of the filter</param>
/// <param name="pbPassFilter">[out] The filter evaluation result.  TRUE means pass; FALSE means not pass</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame
/// </returns>
extern "C" ULONG WINAPI NmEvaluateFilter(__in HANDLE hParsedFrame, __in ULONG ulFilterId, __out PBOOL pbPassFilter);

/// <summary><c>NmGetFieldCount</c>Return number of fields enumerated in the given parsed frame</summary> 
/// <remarks>
/// 
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target frame</param>
/// <param name="pulFieldCount">[out] The number of fields returned in parsed frame</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame
/// </returns>
extern "C" ULONG WINAPI NmGetFieldCount(__in HANDLE hParsedFrame, __out PULONG pulFieldCount);

/// <summary><c>NmGetParsedFieldInfo</c>Return the field information of a parsed frame specified by field index number</summary> 
/// <remarks>
/// The pointer to field is valid until the parsed frame containing the field is closed.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field to get</param>
/// <param name="ulOption">[in] The retrieve flag</param>
/// <param name="pParsedField">[out] The pointer to the parsed field buffer</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
/// </returns>
extern "C" ULONG WINAPI NmGetParsedFieldInfo(__in HANDLE hParsedFrame, __in ULONG ulFieldId, __in ULONG ulOption, __out PNM_PARSED_FIELD_INFO pParsedField);

/// <summary><c>NmGetFieldName</c>Return the names of the parsed field specified by field id</summary> 
/// <remarks>
/// ulBufferLength is element count.
/// </remarks>
/// <example> Description
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field to get</param>
/// <param name="RequestedName">[in] The enum to select intended name property</param>
/// <param name="ulBufferLength">[in] The length of caller provided buffer length</param>
/// <param name="pBuffer">[out] The caller provided buffer</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
///     ERROR_INSUFFICIENT_BUFFER: If ulBufferLength is shorted than the name length
/// </returns>
extern "C" ULONG WINAPI NmGetFieldName(__in HANDLE hParsedFrame,
                                       __in ULONG ulFieldId,
                                       __in NmParsedFieldNames RequestedName,
                                       __in ULONG ulBufferElementCount,
                                       __out_ecount(ulBufferElementCount)LPWSTR pNameBuffer);

/// <summary><c>NmGetFieldOffsetAndSize</c>Return the offset and size of the field specified by field id</summary> 
/// <remarks>
/// The returned field size is in unit of bit
/// </remarks>
/// <example> Description
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field</param>
/// <param name="pulFieldOffset">[out] The pointer to the returned field offset</param>
/// <param name="pulFieldSize">[out] The pointer to the returned field size</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
/// </returns>
extern "C" ULONG WINAPI NmGetFieldOffsetAndSize(__in HANDLE hParsedFrame, __in ULONG ulFieldId, __out PULONG pulFieldOffset, __out PULONG pulFieldSize);

/// <summary><c>NmGetFieldValueNumber8Bit</c>Return number type field value</summary> 
/// <remarks>
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field</param>
/// <param name="pubNumber">[out] The value of the requested field</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
/// </returns>
extern "C" ULONG WINAPI NmGetFieldValueNumber8Bit(__in HANDLE hParsedFrame, __in ULONG ulFieldId, __out PUINT8 pubNumber);

/// <summary><c>NmGetFieldValueNumber16Bit</c>Return number type field value</summary> 
/// <remarks>
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field</param>
/// <param name="puiNumber">[out] The value of the requested field</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
/// </returns>
extern "C" ULONG WINAPI NmGetFieldValueNumber16Bit(__in HANDLE hParsedFrame, __in ULONG ulFieldId, __out PUINT16 puiNumber);

/// <summary><c>NmGetFieldValueNumber32Bit</c>Return number type field value</summary> 
/// <remarks>
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field</param>
/// <param name="pulNumber">[out] The value of the requested field</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
/// </returns>
extern "C" ULONG WINAPI NmGetFieldValueNumber32Bit(__in HANDLE hParsedFrame, __in ULONG ulFieldId, __out PUINT32 pulNumber);

/// <summary><c>NmGetFieldValueNumber64Bit</c>Return number type field value</summary> 
/// <remarks>
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field</param>
/// <param name="pullNumber">[out] The value of the requested field</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
/// </returns>
extern "C" ULONG WINAPI NmGetFieldValueNumber64Bit(__in HANDLE hParsedFrame, __in ULONG ulFieldId, __out PUINT64 pullNumber);

/// <summary><c>NmGetFieldValueByteArray</c>Return byte array type field value</summary> 
/// <remarks>
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field</param>
/// <param name="ulLength">[in] The size of the array in byte</param>
/// <param name="pByteBuffer">[out] The value of the requested field</param>
/// <param name="pulReturnLength">[out] The number of bytes returned in the buffer</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, data is not copied
///     ERROR_RESOURCE_NOT_AVAILABLE: The field is a container so the content is not available
/// </returns>
extern "C" ULONG WINAPI NmGetFieldValueByteArray(__in HANDLE hParsedFrame,
                                                 __in ULONG ulFieldId,
                                                 __in ULONG ulLength,
                                                 __out_bcount(ulLength)PBYTE pByteBuffer,
                                                 __out PULONG pulReturnLength);

/// <summary><c>NmGetFieldValueString</c>Return string array type field value</summary> 
/// <remarks>
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field</param>
/// <param name="ulElementLength">[in] The size of string buffer in element unit</param>
/// <param name="pString">[out] The value of the requested field</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, partial sting is copied.
/// </returns>
extern "C" ULONG WINAPI NmGetFieldValueString(__in HANDLE hParsedFrame,
                                              __in ULONG ulFieldId,
                                              __in ULONG ulElementLength,
                                              __out_ecount(ulElementLength) LPWSTR pString);

/// <summary><c>NmGetFieldInBuffer</c>Get the field in user provided buffer </summary> 
/// <remarks>
/// Only the content up to the buffer length is copied.  Caller may call NmGetFieldOffsetAndSize to get the size 
/// Before calling this function with proper buffer length.  Or call this function twice if first time failed with 
/// ERROR_INSUFFICIENT_BUFFER status.  Allocate new buffer length according to the return length.
///
/// All fields with values can be retrieved.  The return data is in the same order as it on wire.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
/// <param name="ulFieldId">[in] The identify number of the field</param>
/// <param name="ulBufferLength">[in] The length of caller provided buffer</param>
/// <param name="pFieldBuffer">[out] caller provided buffer</param>
/// <param name="pulReturnLength">[out] actual number of byte copied</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame or field
///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, data is not copied.
/// </returns>
extern "C" ULONG WINAPI NmGetFieldInBuffer(__in HANDLE hParsedFrame,
                                           __in ULONG ulFieldId,
                                           __in ULONG ulBufferLength,
                                           __out_bcount(ulBufferLength) PBYTE pFieldBuffer,
                                           __out PULONG pulReturnLength);

/// <summary><c>NmGetRequestedPropertyCount</c>Get the number of properties added to the parser.</summary> 
/// <remarks>
/// None.
/// </remarks>
/// <example> This sample shows how to call the NmGetRequestedPropertyCount method.
/// <code>
///     HANDLE hFrameParser;
///     ULONG propertyCount = 0;
///     NmGetRequestedPropertyCount(hFrameParser, &propertyCount);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParser">[in] Frame Parser Handle</param>
/// <param name="pulCount">[out] Count of properties added to this frame configuration.</param>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle, wrong scope or NULL pointer
///     ERROR_NOT_FOUND: not found specified frame parser
/// </returns>
extern "C" ULONG WINAPI NmGetRequestedPropertyCount(__in HANDLE hFrameParser, __out PULONG pulCount);

/// <summary><c>NmGetPropertyInfo</c>Return info structure for a specific property by ID.</summary> 
/// <remarks>
/// When the property container type is multi-value storage, the value type and size may be unknown if the property name added does not contain the key.
/// Since the size is unknown, caller may need to call the retrieval function twice with the proper buffer of required size returned by the 
/// Retrieval funciton that return ERROR_INSUFFICIENT_BUFFER.  The same is true for array property when the index in not included in the property string
/// Added.
///
/// If the property container type is unknown, the property is not available for retrieval.
/// </remarks>
/// <example> This sample shows how to call the NmGetParsedPropertyInfo method.
/// <code>
///     HANDLE hFrameParser;
///     ULONG PropertyId;
///     PNM_PROPERTY_INFO PropertyInfo;
///
///     PropertyInfo.Size = sizeof(NmPropertyInfo);
///     NmGetPropertyInfo(hFrameParser, PropertyId, &PropertyInfo);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParser">[in] Frame Parser Configuration Handle</param>
/// <param name="ulPropertyId">[in] ID of the property returned from NmAddProperty</param>
/// <param name="pInfo">[out] Count of properties added to this frame configuration.</param>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified property
///     NM_STATUS_API_VERSION_MISMATCHED: NM_PARSED_PROPERTY_INFO version mismatch checked by struct size.
/// </returns>
extern "C" ULONG WINAPI NmGetPropertyInfo(__in HANDLE hFrameParser, __in ULONG ulPropertyId, __out PNM_PROPERTY_INFO pInfo);

/// <summary><c>NmGetPropertyValueById</c>Return property value by ID.</summary> 
/// <remarks>
/// The Key for multi-value storage properties or Index for array properties must not provided at both property name and key index array.
/// The key type must match the type used in NPL parser.
/// </remarks>
/// <example> This sample shows how to call the NmGetPropertyValueById.
/// <code>
///  1. REGULAR PROPERTY VALUE RETRIEVAL EXAMPLE:
///
///     HANDLE hFrameParser;
///     ULONG myPropertyID = 1;
///     WCHAR retPropertyValue[256];
///     ULONG returnLength;
///     NmPropertyValueType propertyType;
///     NmGetPropertyValueById( myFrameParser,
///                             myPropertyID,
///                             256, 
///                             retPropertyValue, 
///                             &returnLength,
///                             &propertyType);
///
///  2. ARRAY PROPERTY VALUE RETRIEVAL EXAMPLE:
///
///     HANDLE hFrameParser;
///     ULONG myNameTableID = 2;
///     PNM_PROPERTY_STORAGE_KEY myKey;
///     myKey.Size = Sizeof(PNM_PROPERTY_STORAGE_KEY);
///     myKey.KeyType = NmMvsKeyTypeArrayIndex;
///     myKey.ArrayIndex = 5;
///     WCHAR retName[256];
///     ULONG returnLength;
///     NmPropertyValueType propertyType;
///     NmGetPropertyValueById( myFrameParser,
///                             myNameTableID,
///                             256, 
///                             retName, 
///                             &returnLength,
///                             &propertyType,
///                             1,
///                             myKey);
///
///  3. MVS PROPERTY RETRIEVAL EXAMPLE:
///
///     HANDLE hFrameParser;
///     ULONG myNameTableID = 3;
///     PNM_PROPERTY_STORAGE_KEY myKey;
///     myKey.Size = Sizeof(PNM_PROPERTY_STORAGE_KEY);
///     myKey.KeyType = NmMvsKeyTypeString;
///     myKey.KeyString = L"192.168.1.1";
///     WCHAR retName[256];
///     ULONG returnLength;
///     NmPropertyValueType propertyType;
///     NmGetPropertyValueById( myFrameParser,
///                             myNameTableID,
///                             256, 
///                             retName, 
///                             &returnLength,
///                             &propertyType,
///                             1,
///                             myKey);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParser">[in] Frame Parser Handle</param>
/// <param name="ulPropertyId">[in] ID of the property returned from NmAddProperty</param>
/// <param name="ulBufferSize">[in] Size of the buffer supplied in byte count.</param>
/// <param name="pBuffer">[out] Buffer for returned data.</param>
/// <param name="pulReturnLength">[out] Size of the data returned.</param>
/// <param name="pType">[out] Value type of the returned MVS property.</param>
/// <param name="ulKeyCount">[in] Number of keys provided</param>
/// <param name="pKeyArray">[in] key Array to look up in MVS and property group </param>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified property
///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, data is not copied.  The required length is returned.
/// </returns>
extern "C" ULONG WINAPI NmGetPropertyValueById(__in HANDLE hFrameParser, 
                                               __in ULONG ulPropertyId, 
                                               __in ULONG ulBufferSize, 
                                               __out_bcount(ulBufferSize) PBYTE pBuffer, 
                                               __out PULONG pulReturnLength, 
                                               __out PNmPropertyValueType pType, 
                                               __in ULONG ulKeyCount = 0, 
                                               __in PNM_PROPERTY_STORAGE_KEY pKeyArray = NULL);

/// <summary><c>NmGetPropertyValueByName</c>Return property value by Name.</summary> 
/// <remarks>
/// The property is not necessarily added to the frame parser configuration if a non-optimized frame parser is used.  In this case, the property id is not available and the
/// The property name can be used.  The full qualified name must be used as to add the property to the frame parser configuration.
/// The key type for multi-value storage must match the type used in NPL parser.
/// </remarks>
/// <example> This sample shows how to call the NmGetPropertyValueByName.
/// <code>
///  1. REGULAR PROPERTY VALUE RETRIEVAL EXAMPLE:
///
///     HANDLE hFrameParser;
///     LPWSTR myPropertyName = L"Global.IamReguler";
///     WCHAR retPropertyValue[256];
///     ULONG returnLength;
///     NmPropertyValueType propertyType;
///     NmGetPropertyValueByName( myFrameParser,
///                               myPropertyName,
///                               256, 
///                               retPropertyValue, 
///                               &returnLength,
///                               &propertyType);
///
///  2. GROUP PROPERTY VALUE RETRIEVAL EXAMPLE:
///
///     HANDLE hFrameParser;
///     ULONG myGroupName = L"Conversation.IamPropertyGroup";
///     PNM_PROPERTY_STORAGE_KEY myKey;
///     myKey.Size = Sizeof(PNM_PROPERTY_STORAGE_KEY);
///     myKey.KeyType = NmMvsKeyTypeArrayIndex;
///     myKey.ArrayIndex = 5;
///     WCHAR retName[256];
///     ULONG returnLength;
///     NmPropertyValueType propertyType;
///     NmGetPropertyValueByName( myFrameParser,
///                               myGroupName,
///                               256, 
///                               retName, 
///                               &returnLength,
///                               &propertyType,
///                               1,
///                               myKey);
///
///  3. MVS PROPERTY RETRIEVAL EXAMPLE:
///
///     HANDLE hFrameParser;
///     ULONG myName = L"Property.IamMvs";
///     PNM_PROPERTY_STORAGE_KEY myKey;
///     myKey.Size = Sizeof(PNM_PROPERTY_STORAGE_KEY);
///     myKey.KeyType = NmMvsKeyTypeNumber;
///     myKey.KeyNumber = 2048;
///     WCHAR retName[256];
///     ULONG returnLength;
///     NmPropertyValueType propertyType;
///     NmGetPropertyValueByName( myFrameParser,
///                               myName,
///                               256, 
///                               retName, 
///                               &returnLength,
///                               &propertyType,
///                               1,
///                               myKey);
///
///  4. MVS PROPERTY RETRIEVAL EXAMPLE 2 (KEY SPECIFIED IN NAME STRING):
///
///     HANDLE hFrameParser;
///     ULONG myName = L"Property.IamMvs$[1000, "\MyId\"]";
///     WCHAR retName[256];
///     ULONG returnLength;
///     NmPropertyValueType propertyType;
///     NmGetPropertyValueByName( myFrameParser,
///                               myName,
///                               256, 
///                               retName, 
///                               &returnLength,
///                               &propertyType);
///
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrameParser">[in] Frame Parser Handle</param>
/// <param name="pPropertyName">[in] full qualified name of the property </param>
/// <param name="ulBufferSize">[in] Size of the buffer supplied in byte count.</param>
/// <param name="pBuffer">[out] Buffer for returned data.</param>
/// <param name="pulReturnLength">[out] Size of the data returned.</param>
/// <param name="pType">[out] Value type of the returned MVS property.</param>
/// <param name="ulKeyCount">[in] Number of keys provided</param>
/// <param name="pKeyArray">[in] key Array to look up in MVS and property group </param>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified property
///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, data is not copied.  The required length is returned.
/// </returns>
extern "C" ULONG WINAPI NmGetPropertyValueByName(__in HANDLE hFrameParser, 
                                                 __in LPCWSTR pPropertyName, 
                                                 __in ULONG ulBufferSize, 
                                                 __out_bcount(ulBufferSize) PBYTE pBuffer, 
                                                 __out PULONG pulReturnLength, 
                                                 __out PNmPropertyValueType pType, 
                                                 __in ULONG ulKeyCount = 0, 
                                                 __in PNM_PROPERTY_STORAGE_KEY pKeyArray = NULL);

/// <summary><c>NmGetRawFrameLength</c>Return length of the raw frame</summary> 
/// <remarks>
/// 
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrame">[in] The handle of the target raw frame</param>
/// <param name="pulLength">[out] Frame length</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified raw frame
/// </returns>
extern "C" ULONG WINAPI NmGetRawFrameLength(__in HANDLE hFrame, __out PULONG pulLength);

/// <summary><c>NmGetRawFrame</c>Copy raw frame to the buffer</summary> 
/// <remarks>
/// The frame buffer is valid until the raw frame is closed.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hRawFrame">[in] The handle of the target raw frame</param>
/// <param name="ulLength">[in] Caller Frame buffer length in byte</param>
/// <param name="pFrameBuffer">[out] caller frame data buffer</param>
/// <param name="pulReturnLength">[out] actual number of byte copied</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified raw frame
///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, partial data is copied.
/// </returns>
extern "C" ULONG WINAPI NmGetRawFrame(__in HANDLE hRawFrame,
                                      __in ULONG ulLength,
                                      __out_bcount(ulLength) PBYTE pFrameBuffer,
                                      __out PULONG pulReturnLength);

/// <summary><c>NmGetPartialRawFrame</c>Return partial frame data in caller provided buffer</summary> 
/// <remarks>
/// Use caller provided offset and buffer length to copy the frame data.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrame">[in] The handle of the target raw frame</param>
/// <param name="ulFrameOffset">[in] Start offset to copy</param>
/// <param name="ulBufferLength">[in] Caller buffer length, the Number of bytes to copy</param>
/// <param name="pFrameBuffer">[out] Caller provided buffer</param>
/// <param name="ulReturnLength">[out] actual number of byte copied</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified raw frame
/// </returns>
extern "C" ULONG WINAPI NmGetPartialRawFrame(__in HANDLE hRawFrame,
                                             __in ULONG ulFrameOffset,
                                             __in ULONG ulBufferLength,
                                             __out_bcount(ulBufferLength) PBYTE pFrameBuffer,
                                             __out PULONG ulReturnLength);

/// <summary><c>NmGetFrameMacType</c>Return MAC type of the frame</summary> 
/// <remarks>
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame"> [in] The handle of the parsed frame object</param>
/// <param name="pulMacType"> [out] The pointer to the MAC type of the frame</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame
/// </returns>
extern "C" ULONG WINAPI NmGetFrameMacType(__in HANDLE hParsedFrame, __out PULONG pulMacType);

/// <summary><c>NmGetFrameTimeStamp</c>Return the absolute time of the capture</summary> 
/// <remarks>
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame"> [in] The handle of the parsed frame object</param>
/// <param name="pTimeStamp"> [out] The pointer to the time stamp of the frame</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified parsed frame
/// </returns>
extern "C" ULONG WINAPI NmGetFrameTimeStamp(__in HANDLE hParsedFrame, __out PUINT64 pTimeStamp);

/// <summary><c>NmGetFrameCommentInfo</c>Return the frame comment title and description</summary>
/// <remarks>
/// If the buffers passed in are NULL, the buffer length arguments will indicate the required lengths.
/// </remarks>
/// <example>
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hFrame"> [in] The handle of the parsed frame object></param>
/// <param name="pulCommentTitleBufferLength"> [inout] The pointer to the actual byte length that corresponds to the title buffer</param>
/// <param name="pCommentTitleBuffer"> [out] Caller supplied buffer to hold the comment title</param>
/// <param name="pulCommentDescriptionBufferLength"> [inout] The pointer to the actual byte length that corresponds to the description buffer</param>
/// <param name="pCommentDescriptionBuffer"> [out] Caller supplied buffer to hold the comment description</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: Specified parsed frame not found
///     ERROR_INSUFFICIENT_BUFFER: If either of the supplied buffers is shorter than the content to retrieve.
///     ERROR_EMPTY: Frame comment information was not found
/// </returns>
extern "C" ULONG WINAPI NmGetFrameCommentInfo(__in HANDLE hFrame, 
                                              __inout PULONG pulCommentTitleBufferLength, 
                                              __out_bcount(pulCommentTitleBufferLength) PBYTE pCommentTitleBuffer, 
                                              __inout PULONG pulCommentDescriptionBufferLength,
                                              __out_bcount(pulCommentDescriptionBufferLength) PBYTE pCommentDescriptionBuffer);


//////////////////////////////////////////////////////
/// Capture file functions
/////////////////////////////////////////////////////
/// <summary><c>NmCreateCaptureFile</c> Create a new Netmon capture file for adding frames.</summary> 
/// <remarks>
/// This is the capture file to write to. Close it by calling NmCloseObjHandle method.
/// The file can be opened in 2 modes specified by Flags, wrap around (default) and chain capture.
/// </remarks>
/// <example> Description
/// <code>
///     
///     
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="pFileName">[in] The name of the file to create</param>
/// <param name="ulSize">[in] The maximum size of the file in byte.  The hard limit is 500 MByte</param>
/// <param name="ulFlags">[in] Specify the file modes, wrap-round or chain capture</param>
/// <param name="phCaptureFile">[out] The returned handle to the capture file object if successful</param>
/// <param name="pulReturnSize">[out] The actual size of the returned file in byte.</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: NULL pointer
///     ERROR_NOT_ENOUGH_MEMORY: not enough memory to build required objects.
/// </returns>
extern "C" ULONG WINAPI NmCreateCaptureFile(__in LPCWSTR pFileName,
                                            __in ULONG ulSize,
                                            __in ULONG ulFlags,
                                            __out PHANDLE phCaptureFile,
                                            __out PULONG pulReturnSize);

/// <summary><c>NmOpenCaptureFile</c> Open a Netmon capture file to read</summary> 
/// <remarks>
/// The file is read only. Close capture file by calling NmCloseObjHandle method.
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="pFileName">[in] The name of the file to open</param>
/// <param name="phCaptureFile">[out] The returned handle of the capture file object if successful</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: NULL pointer
///     ERROR_NOT_FOUND: not found specified file
///     ERROR_NOT_ENOUGH_MEMORY: not enough memory to build required objects.
/// </returns>
extern "C" ULONG WINAPI NmOpenCaptureFile(__in LPCWSTR pFileName, __out PHANDLE phCaptureFile);

/// <summary><c>NmOpenCaptureFileInOrder</c> Open a Netmon capture file to read.  The out of sequence frames are put in order </summary> 
/// <remarks>
/// The file is read only. Close capture file by calling NmCloseObjHandle method.
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="pFileName">[in] The name of the file to open</param>
/// <param name="pOrderParser">[in] The frame parser configured with sequence parameters to handle out of order frames</param>
/// <param name="phCaptureFile">[out] The returned handle of the capture file object if successful</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: NULL pointer
///     ERROR_NOT_FOUND: not found specified file
///     ERROR_INVALID_PARAMETER: frame parser does not have sequence configuration.
///     ERROR_NOT_ENOUGH_MEMORY: not enough memory to build required objects.
///     NM_STATUS_API_VERSION_MISMATCHED: PNM_ORDER_PARSER_PARAMETER version does not match.
/// </returns>
extern "C" ULONG WINAPI NmOpenCaptureFileInOrder(__in LPCWSTR pFileName, __in PNM_ORDER_PARSER_PARAMETER pOrderParser, __out PHANDLE phCaptureFile);

/// <summary><c>NmAddFrame</c> Add a frame to the specified capture file.</summary> 
/// <remarks>
/// The target capture file must be opened with NmCreateCaptureFile method
/// </remarks>
/// <example> 
/// <code>
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hCaptureFile">[in] The destination capture file for the frame</param>
/// <param name="hFrame">[in] The frame to add</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle
///     ERROR_NOT_FOUND: not found specified file or frame
/// </returns>
extern "C" ULONG WINAPI NmAddFrame(__in HANDLE hCaptureFile, __in HANDLE hFrame);

/// <summary><c>NmGetFrameCount</c> Get frame count in the specified capture file</summary> 
/// <remarks>
/// 
/// </remarks>
/// <exception>None</exception>
/// <param name="hCaptureFile">[in] The target capture file under query</param>
/// <param name="pFrameCount">[out] Return frame count</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified capture file
/// </returns>
extern "C" ULONG WINAPI NmGetFrameCount(__in HANDLE hCaptureFile, __out PULONG pFrameCount);

/// <summary><c>NmGetFrame</c> Get frame by number from the specified capture file.</summary> 
/// <remarks>
/// The frame number is the index number in the capture file.
/// </remarks>
/// <exception>None</exception>
/// <param name="hCaptureFile">[in] Handle to the capture file</param>
/// <param name="ulFrameNumber">[in] Frame number in the capture file to retrieve</param>
/// <param name="phFrame">[out] The returned handle to the raw frame object if successful</param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>
///     ERROR_SUCCESS: Frame handle is valid
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_NOT_FOUND: not found specified capture file or frame
/// </returns>
extern "C" ULONG WINAPI NmGetFrame(__in HANDLE hCaptureFile, __in ULONG ulFrameNumber, __out PHANDLE phFrame);

/// <summary><c>NmCloseHandle</c> Release the reference to the object by handle</summary> 
/// <remarks>
/// Callers need to close all the object handles returned from API after finish using them.
/// </remarks>
/// <exception>None</exception>
/// <param name="hObjectHandle"> Handle to the object to release </param>
/// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
/// <returns>Void</returns>
extern "C" VOID WINAPI NmCloseHandle(__in HANDLE hObjectHandle);

//////////////////////////////////////////////
///
/// Conversation Info
///

/// <summary><c>NmGetTopConversation</c>Return the top level conversation and protocol name.</summary> 
/// <remarks>
/// The frame parsed used to parse the frame must have conversation configured as TRUE.
///
/// The protocol name length is returned to caller.  So if the provided buffer is not enough, caller
/// Can call again with the proper sized buffer accordingly.
/// </remarks>
/// <example> This sample shows how to call the NmGetTopConversation method.
/// <code>
///     HANDLE hParsedFrame;
///     WCHAR retName[256];
///     ULONG returnLength;
///     NmGetTopConversation(hParsedFrame, 
///                          256, 
///                          retName, 
///                          &returnLength,
///                          &conversationId);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] Parsed Frame</param>
/// <param name="ulBufferESize">[in] Size of the for protocol name in WCHAR.</param>
/// <param name="pProtocolName">[out] Buffer for protocol name.</param>
/// <param name="pulProtocolNameLength">[out] Not include terminator in WCHAR.</param>
/// <param name="pulConversationID">[out] ID of the TOP Level Conversation</param>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_INSUFFICIENT_BUFFER: Insufficient buffer space
///     ERROR_NOT_FOUND: not found specified parsed frame or the conversation.
/// </returns>
extern "C" ULONG WINAPI NmGetTopConversation(__in HANDLE hParsedFrame,
                                             __in ULONG ulBufferESize,
                                             __out_ecount(ulbufferESize) LPWSTR pProtocolName,
                                             __out PULONG pulProtocolNameLength,
                                             __out PULONG pulConversationID);

/// <summary><c>NmGetParentConversation</c>Return parent conversation information of the given conversation.</summary> 
/// <remarks>
/// The frame parsed used to parse the frame must have conversation configured as TRUE.
///
/// The parent protocol name length is returned to caller.  So if the provided buffer is not enough, caller
/// Can call again with the proper sized buffer.
/// </remarks>
/// <example> This sample shows how to call the NmGetParentConversation method.
/// <code>
///     HANDLE myParsedFrame;
///     ULONG protocolId = 24;
///     WCHAR retName[256];
///     ULONG returnLength;
///     ULONG parentConvID;
///     NmGetParentConversation(myParsedFrame,
///                             protocolId,
///                             256, 
///                             retName, 
///                             &returnLength,
///                             &parentConvID);
/// </code>
/// </example>
///
/// <exception>None</exception>
/// <param name="hParsedFrame">[in] Parsed Frame</param>
/// <param name="ulConversationID">[in] ID of the Conversation you want the parent of.</param>
/// <param name="ulBufferESize">[in] Buffer size for the Parent protocol name in WCHAR count.</param>
/// <param name="pParentProtocolName">[out] Buffer for the Parent Protocol Name. </param>
/// <param name="pulParentProtocolNameLength">[out] Returned Length of Parent Protocol Name in WCHAR.</param>
/// <param name="pulParentConversationID">[out] Size of the for protocol name.</param>
/// <returns>
///     ERROR_SUCCESS:
///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
///     ERROR_INSUFFICIENT_BUFFER: Insufficient buffer space
///     ERROR_NOT_FOUND: not found specified frame parser or the conversation.
/// </returns>
extern "C" ULONG WINAPI NmGetParentConversation(__in HANDLE hParsedFrame,
                                                __in ULONG ulConversationId,
                                                __in ULONG ulBufferESize,
                                                __out_ecount(ulBufferESize) LPWSTR pParentProtocolNameBuffer,
                                                __out PULONG pulParentProtocolNameLength, 
                                                __out PULONG pulParentConversationID);

#endif /// __NM_API_HEADER__
