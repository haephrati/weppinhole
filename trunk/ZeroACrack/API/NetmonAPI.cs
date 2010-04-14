using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Microsoft.NetworkMonitor
{
    #region Netmon API structs and defines

    /// <summary>
    /// Netmon API Constants
    /// </summary>
    public class NmConstant
    {
        /// <summary>
        /// 
        /// </summary>
        public const int MAC_ADDRESS_SIZE = 6;
        /// <summary>
        /// 
        /// </summary>
        public const int MAX_PATH = 260;
        /// <summary>
        /// 
        /// </summary>
        public const int NMAPI_GUID_SIZE = 16;
    }

    /// <summary>
    /// Netmon API return status code
    /// </summary>
    public class NmStatusCode
    {
        /// <summary>
        /// MessageId: NM_STATUS_FRAME_TOO_BIG_FOR_FILE
        ///
        /// MessageText:
        ///
        /// The file doesn't have enough space to hold this frame.
        /// </summary>
        public const UInt32 NM_STATUS_FRAME_TOO_BIG_FOR_FILE = 0xE1110001;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_FILE_TOO_SMALL
        ///
        /// MessageText:
        ///
        /// Capture file size too small. 
        ///
        /// </summary>
        public const UInt32 NM_STATUS_FILE_TOO_SMALL = 0xE1110002;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_FILE_TOO_LARGE
        ///
        /// MessageText:
        ///
        /// Capture file size too large. 
        ///
        /// </summary>
        public const UInt32 NM_STATUS_FILE_TOO_LARGE = 0xE1110003;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_FRAME_CONTINUES_INTO_NEXT_FRAME
        ///
        /// MessageText:
        ///
        /// The frame is corrupt. It overlaps with the next frame. 
        ///
        /// </summary>
        public const UInt32 NM_STATUS_FRAME_CONTINUES_INTO_NEXT_FRAME = 0xE1110004;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_FRAME_RANGE_OUT_OF_BOUNDS
        ///
        /// MessageText:
        ///
        /// The frame is corrupt. The dimensions of the frame are not in the range of the capture file. 
        ///
        /// </summary>
        public const UInt32 NM_STATUS_FRAME_RANGE_OUT_OF_BOUNDS = 0xE1110005;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_WRONG_ENDIAN
        ///
        /// MessageText:
        ///
        /// The data is in BigEndian and we support only Little Endian 
        ///
        /// </summary>
        public const UInt32 NM_STATUS_WRONG_ENDIAN = 0xE1110006;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_INVALID_PCAP_FILE
        ///
        /// MessageText:
        ///
        /// This file is not a valid PCAP file
        ///
        /// </summary>
        public const UInt32 NM_STATUS_INVALID_PCAP_FILE = 0xE1110007;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_WRONG_PCAP_VERSION
        ///
        /// MessageText:
        ///
        /// This file is not a supported PCAP version 
        ///
        /// </summary>
        public const UInt32 NM_STATUS_WRONG_PCAP_VERSION = 0xE1110008;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_UNSUPPORTED_FILE_TYPE
        ///
        /// MessageText:
        ///
        /// This file type is not supported.
        ///
        /// </summary>
        public const UInt32 NM_STATUS_UNSUPPORTED_FILE_TYPE = 0xE1110009;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_INVALID_NETMON_CAP_FILE
        ///
        /// MessageText:
        ///
        /// This file type is not a valid Netmon capture file.
        ///
        /// </summary>
        public const UInt32 NM_STATUS_INVALID_NETMON_CAP_FILE = 0xE111000A;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_UNSUPPORTED_PCAP_DLT
        ///
        /// MessageText:
        ///
        /// This Pcap data link type is not supported.
        ///
        /// </summary>
        public const UInt32 NM_STATUS_UNSUPPORTED_PCAP_DLT = 0xE111000B;

        /// <summary>
        ///
        /// MessageId: NM_STATUS_API_VERSION_MISMATCHED
        ///
        /// MessageText:
        ///
        /// The current NmApi DLL is different from the required version by the application. 
        ///
        /// </summary>
        public const UInt32 NM_STATUS_API_VERSION_MISMATCHED = 0xE111000C;
    }

    ///
    /// <summary><c>NmCaptureMode</c>Netmon capture mode</summary> 
    /// <remarks>
    /// </remarks>
    public enum NmCaptureMode : uint 
    {
        /// <summary>
        /// 
        /// </summary>
        LocalOnly,
        /// <summary>
        /// 
        /// </summary>
        Promiscuous
    }

    ///
    /// <summary><c>NmCaptureFileMode</c>Netmon capture file expansion mode</summary> 
    /// <remarks>
    /// </remarks>
    public enum NmCaptureFileFlag : uint
    {
        /// <summary>
        /// 
        /// </summary>
        WrapAround,
        /// <summary>
        /// 
        /// </summary>
        Chain,
        /// <summary>
        /// 
        /// </summary>
        LastFlag
    };

    ///
    /// <summary><c>NmCaptureCallbackExitMode</c>Capture callback function exit mode</summary> 
    /// <remarks>
    ///     NmCaptureStopAndDiscard - NmStopCapture/NmPauseCapture returns immediately user's capture callback function will not be called after
    ///                               NmStopCapture/NmPauseCapture returns
    /// </remarks>
    public enum NmCaptureCallbackExitMode : uint
    {
        /// <summary>
        /// 
        /// </summary>
        DiscardRemainFrames = 1,
        /// <summary>
        /// 
        /// </summary>
        ReturnRemainFrames = 2,
    };

    ///
    /// <summary><c>NmAdapterOpState</c>Netmon driver adapter operational states</summary> 
    /// <remarks>
    /// </remarks>
    public enum NmAdapterOpState : uint
    {
        /// <summary>
        /// 
        /// </summary>
        None = 0,
        /// <summary>
        /// 
        /// </summary>
        Bound,
        /// <summary>
        /// 
        /// </summary>
        Stopped,
        /// <summary>
        /// 
        /// </summary>
        Capturing,
        /// <summary>
        /// 
        /// </summary>
        Paused
    };


    ///
    /// <summary><c>NmCallbackMsgType</c>Status levels of the call back message</summary> 
    /// <remarks>
    /// </remarks>
    public enum NmCallbackMsgType : uint
    {
        /// <summary>
        /// 
        /// </summary>
        None,
        /// <summary>
        /// 
        /// </summary>
        Error,
        /// <summary>
        /// 
        /// </summary>
        Warning,
        /// <summary>
        /// 
        /// </summary>
        Information,
        /// <summary>
        /// 
        /// </summary>
        Last
    };

    ///
    /// <summary><c>NmNplParserLoadingOption</c>NPL loading option</summary> 
    /// <remarks>
    /// By default the NmLoadNplOptionNone is used.  Only the user specified NPL path(s) are loaded.
    /// If both NmAppendRegisteredNplSets and a NPL path are specified, the resulting NPL parser will include
    /// Both and the specified NPL path(s) are prefixed.
    /// </remarks>
    public enum NmNplParserLoadingOption : uint
    {
        NmLoadNplOptionNone,
        NmAppendRegisteredNplSets
    };

    ///
    /// <summary><c>NmFrameParserOptimizeOption</c>Frame parser optimization options</summary> 
    /// <remarks>
    /// Options used when create frame parser.
    /// </remarks>
    public enum NmFrameParserOptimizeOption : uint
    {
        ///
        /// Create frame parser without optimization according to the added fitler
        ///
        ParserOptimizeNone = 0,
        ///
        /// Create frame parser optimized based on added filters, fields and properties
        ///
        ParserOptimizeFull = 1,

        ParserOptimizeLast

    };

    ///
    /// <summary><c>NmFrameParsingOption</c>Frame parser parsing options</summary> 
    /// <remarks>
    /// Options used by NmParseFrame function.
    /// </remarks>
    [Flags]
    public enum NmFrameParsingOption : uint
    {
        /// <summary>
        /// 
        /// </summary>
        None = 0,
        /// <summary>
        /// Provide full path name of the current field if specified
        /// </summary>
        FieldFullNameRequired = 1,
        /// <summary>
        /// Provide the name of the protocol that contains the current field if specified
        /// </summary>
        ContainingProtocolNameRequired = 2,
        /// <summary>
        /// Provide data type name of the fields
        /// </summary>
        DataTypeNameRequired = 4,
        /// <summary>
        /// Use caller specified frame number
        /// </summary>
        UseFrameNumberParameter = 8,
        ///
        /// Provide the display string of the field
        ///
        FieldDisplayStringRequired = 16,
        ///
        /// Provide the frame conversation information
        ///
        FrameConversationInfoRequired = 32,
        /// <summary>
        /// 
        /// </summary>
        ParsingOptionLast

    };


    ///
    /// <summary><c>FrameFragmentationType</c>Fragmentation types returned in parsed frames</summary> 
    /// <remarks>
    /// </remarks>
    public enum NmFrameFragmentationType : uint
    {
        /// <summary></summary>
        None,
        /// <summary></summary>
        Start,
        /// <summary></summary>
        Middle,
        /// <summary></summary>
        End

    }

    ///
    /// <summary><c>NmParsedFieldNames</c>The name string properties in parsed field</summary> 
    /// <remarks>
    /// </remarks>
    public enum NmParsedFieldNames : uint
    {
        /// <summary></summary>
        NamePath,
        /// <summary></summary>
        DataTypeName,
        /// <summary></summary>
        ContainingProtocolName,
        /// <summary></summary>
        FieldDisplayString
    };

    ///
    /// <summary><c>NmMvsKeyType</c>Key types of the multi storage property</summary> 
    /// <remarks>
    /// The MvsKeyTypeArrayIndex type is used for group property functions to specify the index parameter.
    /// </remarks>
    public enum NmMvsKeyType : uint
    {
        /// <summary></summary>
        MvsKeyTypeNone,
        /// <summary></summary>
        MvsKeyTypeNumber,
        /// <summary></summary>
        MvsKeyTypeString,
        /// <summary></summary>
        MvsKeyTypeByteArray,
        /// <summary></summary>
        MvsKeyTypeArrayIndex,
        /// <summary></summary>
        MvsKeyTypeLast

    };

    ///
    /// <summary>
    /// <c>NmPropertyScope</c>
    /// Scopes of properties.  It is reported in the property info.
    /// </summary> 
    /// <remarks>
    /// </remarks>
    ///
    public enum NmPropertyScope : uint
    {
        /// <summary></summary>
        PropertyScopeNone = 0,
        /// <summary></summary>
        PropertyScopeConversation = 1,
        /// <summary></summary>
        PropertyScopeGlobal = 2,
        /// <summary></summary>
        PropertyScopeFrame = 4

    };

    ///
    /// <summary>
    /// <c>NmPropertyContainerType</c>
    /// The property aggregation form, i.e., MVS with key, Array with index, etc.
    /// </summary> 
    /// <remarks>
    /// </remarks>
    ///
    public enum NmPropertyContainerType : uint
    {
        /// <summary></summary>
        PropertyTypeContainerNone = 0,
        /// <summary></summary>
        PropertyTypeContainerValue,
        /// <summary></summary>
        PropertyTypeContainerMvs,
        /// <summary></summary>
        PropertyTypeContainerArray

    };

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
    /// .
    /// </remarks>
    ///
    public enum NmPropertyValueType : uint
    {
        /// <summary></summary>
        PropertyValueNone,
        /// <summary></summary>
        PropertyValueSignedNumber,
        /// <summary></summary>
        PropertyValueUnsignedNumber,
        /// <summary></summary>
        PropertyValueString,
        /// <summary></summary>
        PropertyValueByteBlob

    };

    ///
    /// <summary><c>CNmPropertyStorageKey</c></summary> 
    /// <remarks>
    /// The top three fields are marshaled to native code.
    /// Create key using the provided methods according to the key types 
    /// (number, string or byte array.)
    /// </remarks>
    /// <example>
    /// <code>
    /// CNmPropertyStorageKey [] keyArray = new CNmPropertyStorageKey[3];
    /// CNmPropertyStorageKey key0 = new CNmPropertyStorageKey();
    /// CNmPropertyStorageKey key1 = new CNmPropertyStorageKey();
    /// CNmPropertyStorageKey key2 = new CNmPropertyStorageKey();
    /// NmPropertyValueType vType;
    /// int myInt;
    /// // Build a Number key
    /// key0.SetNumberKey(0x9a);
    /// // Build a string key
    /// key1.SetStringKey("MyKey");
    /// // build a blob key
    /// byte [] keyBuffer = new byte[16];
    /// for (int i = 0; i < 16; ++i)
    /// {
    ///    keyBuffer[i] = (byte)i;
    /// }
    /// key2.SetByteArrayKey(keyBuffer, 16);
    /// keyArray[0] = key0;
    /// keyArray[1] = key1;
    /// keyArray[2] = key2;
    /// </code>
    /// </example>
    public struct CNmPropertyStorageKey
    {
        public NmMvsKeyType Type;
        public int Length;
        public System.IntPtr ValuePointer;

        /// <summary>
        /// For multi-value storage key in string form
        /// </summary>
        public void SetStringKey(String KeyValue)
        {
            Type = NmMvsKeyType.MvsKeyTypeString;
            ValuePointer = Marshal.StringToHGlobalUni(KeyValue);
            Length = KeyValue.Length;
        }

        /// <summary>
        /// For multi-value storage key in numeric form (64-bit)
        /// </summary>
        public void SetNumberKey(Int64 KeyValue)
        {
            Type = NmMvsKeyType.MvsKeyTypeNumber;
            ValuePointer = Marshal.AllocHGlobal(8);
            Marshal.WriteInt64(ValuePointer, KeyValue);
            Length = 8;
        }

        /// <summary>
        /// For property in Array
        /// </summary>
        public void SetIndexKey(Int64 KeyValue)
        {
            Type = NmMvsKeyType.MvsKeyTypeArrayIndex;
            ValuePointer = Marshal.AllocHGlobal(8);
            Marshal.WriteInt64(ValuePointer, KeyValue);
            Length = 8;
        }

        /// <summary>
        /// For multi-value storage key in byte array form
        /// </summary>
        public void SetByteArrayKey(byte[] KeyValue, Int32 KeyLength)
        {
            Type = NmMvsKeyType.MvsKeyTypeByteArray;
            ValuePointer = Marshal.AllocHGlobal(KeyLength);
            for (int i = 0; i < KeyLength; ++i)
            {
                Marshal.WriteByte(ValuePointer, i, KeyValue[i]);
            }
            Length = KeyLength;
        }
    };

    ///
    /// <summary><c>NM_NPL_PROPERTY_INFO</c></summary> 
    /// <remarks>
    /// Contains runtime information for instantiated properties
    /// </remarks>
    ///
    public struct NM_NPL_PROPERTY_INFO
    {
        /// <summary>
        /// For version control
        /// </summary>
        public System.UInt16 Size;
        /// <summary>
        /// Property Scope
        /// </summary>
        public NmPropertyScope Scope;
        /// <summary>
        /// Property container type, e.g., MVS, Array.
        /// </summary>
        public NmPropertyContainerType ContainerType;
        /// <summary>
        /// The element count of the name excluding the terminator
        /// </summary>
        public System.UInt16 NameSize;
        /// <summary>
        /// Property string added by NmAddProperty
        /// </summary>
        [MarshalAs(UnmanagedType.LPWStr)]
        public System.String Name;
        /// <summary>
        /// The data type of the property
        /// </summary>
        public NmPropertyValueType ValueType;
        /// <summary>
        /// The size of the value.  If the value type is string, the terminator is not included.
        /// </summary>
        public System.UInt32 ValueSize;
        /// <summary>
        /// number of items in Array.
        /// </summary>
        public System.UInt32 ItemCount;

    };

    ///
    /// <summary><c>NmFragmentationInfo</c>Fragmentation information returned in parsed frames</summary> 
    /// <remarks>
    /// </remarks>
    public struct NmFragmentationInfo
    {
        /// <summary>
        /// </summary>
        public UInt16 Size;

        /// <summary>
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U2, SizeConst = NmConstant.MAX_PATH)]
        public System.Char[] FragmentedProtocolName;

        /// <summary>
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U2, SizeConst = NmConstant.MAX_PATH)]
        public System.Char[] PayloadProtocolName;

        /// <summary>
        /// </summary>
        public NmFrameFragmentationType FragmentType;
    };

    /// <summary>
    /// Callback function for frame receiving/retrieving
    /// </summary>
    public delegate void CaptureCallbackDelegate(IntPtr hCaptureEngine,
                                                 UInt32 ulAdapterIndex,
                                                 IntPtr pCallerContext,
                                                 IntPtr hFrame);



    ///
    /// <summary><c>NmConversationOption</c>Frame parser conversation configuration options</summary> 
    /// <remarks>
    /// </remarks>
    public enum NmConversationConfigOption : uint
    {
        /// <summary>
        /// </summary>
        None,
        /// <summary>
        /// </summary>
        Last
    };

    ///
    /// <summary><c>NmReassemblyOption</c>Frame parser reassembly configuration options</summary> 
    /// <remarks>
    /// </remarks>
    public enum NmReassemblyConfigOption : uint
    {
        /// <summary>
        /// </summary>
        None,
        /// <summary>
        /// </summary>
        Last
    };

    /// <summary>
    /// Callback function for parser compile/build process.
    /// </summary>
    public delegate void ParserCallbackDelegate(IntPtr pCallerContext,
                                                UInt32 ulStatusCode,
                                                [MarshalAs(UnmanagedType.LPWStr)] String lpDescription,
                                                NmCallbackMsgType ulType);

    /// <summary>
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct NM_NIC_ADAPTER_INFO
    {
        /// <summary>
        /// </summary>
        public System.UInt16 Size;

        /// <summary>
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst = NmConstant.MAC_ADDRESS_SIZE)]
        public System.Byte[] PermanentAddr;
        /// <summary>
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst = NmConstant.MAC_ADDRESS_SIZE)]
        public System.Byte[] CurrentAddr;

        /// <summary>
        /// </summary>
        public NDIS_MEDIUM MediaType;

        /// <summary>
        /// </summary>
        public NDIS_PHYSICAL_MEDIUM PhysicalMediaType;

        /// <summary>
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U2, SizeConst = NmConstant.MAX_PATH)]
        public System.Char[] ConnectionName;
        /// <summary>
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U2, SizeConst = NmConstant.MAX_PATH)]
        public System.Char[] FriendlyName;
        /// <summary>
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U2, SizeConst = NmConstant.MAX_PATH)]
        public System.Char[] Guid;

        ///
        /// Network adapter operational state. Indicates if the network adapter is bound, capturing, pause or stopped
        ///
        public NmAdapterOpState OpState;
        ///
        /// Indicates if the network adapter is enabled or disabled. It only can be enabled if it is bound to the netmon driver
        ///
        public System.Int32 Enabled;

        /// <summary>
        /// </summary>
        public System.Int32 PModeEnabled;

        ///
        /// Frame indication callback is assigned by adapter
        ///
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public CaptureCallbackDelegate CallBackFunction;
    };

    /// <summary>
    ///
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct NM_API_CONFIGURATION
    {
        ///
        /// Configurable limits that overwrite default API settings 
        ///
        public System.UInt16 Size;
        /// <summary>
        /// </summary>
        public System.UInt32 RawFrameHandleCountLimit;
        /// <summary>
        /// </summary>
        public System.UInt32 ParsedFrameHandleCountLimit;
        /// <summary>
        /// </summary>
        public System.UInt32 CaptureEngineCountLimit;
        /// <summary>
        /// </summary>
        public System.UInt32 NplParserCountLimit;
        /// <summary>
        /// </summary>
        public System.UInt32 FrameParserConfigCountLimit;
        /// <summary>
        /// </summary>
        public System.UInt32 FrameParserCountLimit;
        /// <summary>
        /// </summary>
        public System.UInt32 CaptureFileCountLimit;

        ///
        /// API threading mode for COM initialization.  
        /// Only support COINIT_MULTITHREADED and COINIT_APARTMENTTHREADED
        ///
        public System.UInt16 ThreadingMode;

        ///
        /// Configurable default feature/behavior parameters
        ///
        public NmConversationConfigOption ConversationOption;
        /// <summary>
        /// </summary>
        public NmReassemblyConfigOption ReassemblyOption;
        /// <summary>
        /// </summary>
        public NmCaptureFileFlag CaptureFileMode;
        /// <summary>
        /// </summary>
        public NmFrameParsingOption FrameParsingOption;
        /// <summary>
        /// </summary>
        public NmCaptureCallbackExitMode CaptureCallbackExitMode;

        ///
        /// Hard limits the API enforce (not configurable)
        ///
        public System.UInt32 MaxCaptureFileSize;
        /// <summary>
        /// </summary>
        public System.UInt32 MinCaptureFileSize;

        /// Maximum number of handles per handle type  
        public System.UInt32 MaxApiHandleLimit;
    };

    ///
    /// <summary><c>NM_PROTOCOL_SEQUENCE_CONFIG</c>Data structure for API user to specify NPL properties and fields
    ///                                            For frame order correction support.
    /// </summary> 
    /// <remarks>
    /// </remarks>
    ///
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct NM_PROTOCOL_SEQUENCE_CONFIG
    {
        ///
        /// API verifies the member 'Size' against the size of its version.  They must match.
        ///
        public UInt16 Size;

        ///
        /// The names of the properties containing the values to form the key 
        /// to identify the group of the frames to get in order.  If multiple names are used,
        /// They are separated by semicolons.  The string must be NULL terminated.
        ///
        [MarshalAs(UnmanagedType.LPWStr)]
        public String GroupKeyString;

        ///
        /// The name of the property containing the frame's sequence number.
        ///
        [MarshalAs(UnmanagedType.LPWStr)]
        public String SequencePropertyString;

        ///
        /// The name of the property containing the frame's next sequence number.
        ///
        [MarshalAs(UnmanagedType.LPWStr)]
        public String NextSequencePropertyString;

    };

    ///
    /// <summary><c>NM_ORDER_PARSER_PARAMETER</c>Data structure for calling NmOpCaptureFileInOrder</summary>
    /// 
    /// <remarks>
    /// </remarks>
    ///
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct NM_ORDER_PARSER_PARAMETER
    {
        ///
        /// API verifies the member 'Size' against the size of its version.  They must match.
        ///
        public UInt16 Size;

        ///
        /// The frame parser used for handling out of order frames.  It must be built from a frame parser
        /// Configuration that has sequence information specified by NM_PROTOCOL_SEQUENCE_CONFIG.
        ///
        public IntPtr hFrameParser;

        ///
        /// For future option flags.
        ///
        public UInt32 Option;

    };

    /// <summary>
    /// </summary>
    public enum NDIS_MEDIUM
    {
        /// <summary>
        /// </summary>
        Ndis_802_3,
        /// <summary>
        /// </summary>
        Ndis_802_5,
        /// <summary>
        /// </summary>
        Ndis_Fddi,
        /// <summary>
        /// </summary>
        Ndis_Wan,
        /// <summary>
        /// </summary>
        Ndis_LocalTalk,
        /// <summary>
        /// </summary>
        Ndis_Dix,              // defined for convenience, not a real medium
        /// <summary>
        /// </summary>
        Ndis_ArcnetRaw,
        /// <summary>
        /// </summary>
        Ndis_Arcnet878_2,
        /// <summary>
        /// </summary>
        Ndis_Atm,
        /// <summary>
        /// </summary>
        Ndis_WirelessWan,
        /// <summary>
        /// </summary>
        Ndis_Irda,
        /// <summary>
        /// </summary>
        Ndis_Bpc,
        /// <summary>
        /// </summary>
        Ndis_CoWan,
        /// <summary>
        /// </summary>
        Ndis_1394,
        /// <summary>
        /// </summary>
        Ndis_InfiniBand,
        /// <summary>
        /// #if ((NTDDI_VERSION >= NTDDI_VISTA) || NDIS_SUPPORT_NDIS6)
        /// </summary>
        Ndis_Tunnel,
        /// <summary>
        /// </summary>
        Ndis_Native802_11,
        /// <summary>
        /// </summary>
        Ndis_Loopback,
        /// <summary>
        /// #endif // (NTDDI_VERSION >= NTDDI_VISTA)
        /// </summary>
        NdisMediumMax               // Not a real medium, defined as an upper-bound
    };


    /// <summary>
    /// </summary>
    public enum NDIS_PHYSICAL_MEDIUM
    {
        /// <summary>
        /// </summary>
        Ndis_Unspecified,
        /// <summary>
        /// </summary>
        Ndis_WirelessLan,
        /// <summary>
        /// </summary>
        Ndis_CableModem,
        /// <summary>
        /// </summary>
        Ndis_PhoneLine,
        /// <summary>
        /// </summary>
        Ndis_PowerLine,
        /// <summary>
        /// includes ADSL and UADSL (G.Lite)
        /// </summary>
        Ndis_DSL,
        /// <summary>
        /// </summary>
        Ndis_FibreChannel,
        /// <summary>
        /// </summary>
        Ndis_1394,
        /// <summary>
        /// </summary>
        Ndis_WirelessWan,
        /// <summary>
        /// </summary>
        Ndis_Native802_11,
        /// <summary>
        /// </summary>
        Ndis_Bluetooth,
        /// <summary>
        /// </summary>
        Ndis_Infiniband,
        /// <summary>
        /// </summary>
        Ndis_WiMax,
        /// <summary>
        /// </summary>
        Ndis_UWB,
        /// <summary>
        /// </summary>
        Ndis_802_3,
        /// <summary>
        /// </summary>
        Ndis_802_5,
        /// <summary>
        /// </summary>
        Ndis_Irda,
        /// <summary>
        /// </summary>
        Ndis_WiredWAN,
        /// <summary>
        /// </summary>
        Ndis_WiredCoWan,
        /// <summary>
        /// </summary>
        Ndis_Other,
        /// <summary>
        /// </summary>
        NdisPhysicalMediumMax       // Not a real physical type, defined as an upper-bound
    };

    /// <summary>
    /// http://www.marin.clara.net/COM/variant_type_definitions.htm
    /// </summary>
    public class FieldType
    {
        /// <summary>
        /// </summary>
        public const UInt16 VT_EMPTY = 0;
        /// <summary>
        /// </summary>
        public const UInt16 VT_NULL = 1;
        /// <summary>
        /// </summary>
        public const UInt16 VT_I2 = 2;
        /// <summary>
        /// </summary>
        public const UInt16 VT_I4 = 3;
        /// <summary>
        /// </summary>
        public const UInt16 VT_R4 = 4;
        /// <summary>
        /// </summary>
        public const UInt16 VT_R8 = 5;
        /// <summary>
        /// </summary>
        public const UInt16 VT_CY = 6;
        /// <summary>
        /// </summary>
        public const UInt16 VT_DATE = 7;
        /// <summary>
        /// </summary>
        public const UInt16 VT_BSTR = 8;
        /// <summary>
        /// </summary>
        public const UInt16 VT_DISPATCH = 9;
        /// <summary>
        /// </summary>
        public const UInt16 VT_ERROR = 10;
        /// <summary>
        /// </summary>
        public const UInt16 VT_BOOL = 11;
        /// <summary>
        /// </summary>
        public const UInt16 VT_VARIANT = 12;
        /// <summary>
        /// </summary>
        /// <summary>
        /// </summary>
        public const UInt16 VT_UNKNOWN = 13;
        /// <summary>
        /// </summary>
        public const UInt16 VT_DECIMAL = 14;
        /// <summary>
        /// </summary>
        public const UInt16 VT_I1 = 16;
        /// <summary>
        /// </summary>
        public const UInt16 VT_UI1 = 17;
        /// <summary>
        /// </summary>
        public const UInt16 VT_UI2 = 18;
        /// <summary>
        /// </summary>
        public const UInt16 VT_UI4 = 19;
        /// <summary>
        /// </summary>
        public const UInt16 VT_I8 = 20;
        /// <summary>
        /// </summary>
        public const UInt16 VT_UI8 = 21;
        /// <summary>
        /// </summary>
        public const UInt16 VT_INT = 22;
        /// <summary>
        /// </summary>
        public const UInt16 VT_UINT = 23;
        /// <summary>
        /// </summary>
        public const UInt16 VT_VOID = 24;

        /// <summary>
        /// </summary>
        public const UInt16 VT_HRESULT = 25;
        /// <summary>
        /// </summary>
        public const UInt16 VT_PTR = 26;
        /// <summary>
        /// </summary>
        public const UInt16 VT_SAFEARRAY = 27;

        /// <summary>
        /// </summary>
        public const UInt16 VT_CARRAY = 28;
        /// <summary>
        /// </summary>
        public const UInt16 VT_USERDEFINED = 29;
        /// <summary>
        /// </summary>
        public const UInt16 VT_LPSTR = 30;

        /// <summary>
        /// </summary>
        public const UInt16 VT_LPWSTR = 31;
        /// <summary>
        /// </summary>
        public const UInt16 VT_FILETIME = 64;
        /// <summary>
        /// </summary>
        public const UInt16 VT_BLOB = 65;

        /// <summary>
        /// </summary>
        public const UInt16 VT_STREAM = 66;
        /// <summary>
        /// </summary>
        public const UInt16 VT_STORAGE = 67;
        /// <summary>
        /// </summary>
        public const UInt16 VT_STREAMED_OBJECT = 68;

        /// <summary>
        /// </summary>
        public const UInt16 VT_STORED_OBJECT = 69;
        /// <summary>
        /// </summary>
        public const UInt16 VT_BLOB_OBJECT = 70;
        /// <summary>
        /// </summary>
        public const UInt16 VT_CF = 71;

        /// <summary>
        /// </summary>
        public const UInt16 VT_CLSID = 72;
        /// <summary>
        /// </summary>
        public const UInt16 VT_VECTOR = 0x1000;
        /// <summary>
        /// </summary>
        public const UInt16 VT_ARRAY = 0x2000;

        /// <summary>
        /// </summary>
        public const UInt16 VT_BYREF = 0x4000;
        /// <summary>
        /// </summary>
        public const UInt16 VT_RESERVED = 0x8000;
        /// <summary>
        /// </summary>
        public const UInt16 VT_ILLEGAL = 0xffff;

        /// <summary>
        /// </summary>
        public const UInt16 VT_ILLEGALMASKED = 0xfff;
        /// <summary>
        /// </summary>
        public const UInt16 VT_TYPEMASK = 0xfff;
    }

    ///
    /// Returned to caller from NmGetParsedField function
    ///     API set the member 'Size' when return for struct version checking purpose
    ///     Member 'NplDataTypeNameLength' and 'ProtocolNameLength' are 0 if not requested by caller when 
    ///     Invoke parsing function.  All string length parameters are in element, e.g., in unicode here.
    ///
    [StructLayout(LayoutKind.Sequential, Pack = 8)]
    public struct NmParsedFieldInfo
    {
        ///
        /// API set the member 'Size' when return for struct version checking purpose
        ///
        public System.UInt16 Size;
        ///
        /// The relative level to the root protocol
        ///
        public System.UInt16 FieldIndent;
        ///
        /// The size of the string that holds the full path of the data field if the NmFrameParseOptions 
        /// FieldFullNameRequired is set, e.g., Frame.Ethernet.IPv4.SourceAddress;  Otherwise it is the size
        /// of the current field name only
        ///
        public System.UInt16 NamePathLength;
        ///
        /// The size of the string that contains the name of the NPL data type if the NmFrameParseOptions 
        /// DataTypeNameRequired is set, e.g., L"UINT16";  Otherwise it is zero.
        ///
        public System.UInt16 NplDataTypeNameLength;
        ///
        /// The size of the string tht contains the protocol containing the field if the NmFrameParseOptions 
        /// ContainingProtocolNameRequired is set;  Otherwise it is zero
        ///
        public System.UInt16 ProtocolNameLength;
        ///
        /// The size of the display string of the field if the NmFrameParseOptions 
        /// FieldDisplayStringRequired is set;  Otherwise it is zero
        ///
        public System.UInt16 DisplayStringLength;
        ///
        /// Offset in current protocol
        ///
        public System.UInt32 ProtocolBitOffset;
        ///
        /// Field offset in frame
        ///
        public System.UInt32 FrameBitOffset;
        ///
        /// Length of the field
        ///
        public System.UInt32 FieldBitLength;
        ///
        /// The variant type defined as in VARENUM
        ///
        public System.UInt16 ValueType;
        ///
        /// The size of the buffer required to hold the field value represented in VARIANT struct including
        /// The length of the content if the VARIANT contains a pointer to ARRAY or string.
        ///
        public System.UInt16 ValueBufferLength;
    };

    #endregion

    /// <summary>
    /// PInvoke wrapper of Netmon API.
    ///     - managed/unmanaged data type mapping: http://msdn2.microsoft.com/en-us/library/ac7ay120.aspx
    ///     - the declaration in this class are strongly tied to nmapi.h
    /// </summary>
    public class NetmonAPI
    {
        #region API Operations

        /// <summary><c>NmGetApiVersion</c>Query current version</summary> 
        /// <remarks>
        /// The API version matches netmon engine version.
        /// </remarks>
        /// <example>
        /// <code>
        ///     UInt16 majorNumber = 0;
        ///     UInt16 minorNumber = 0;
        ///     UInt16 BuildNumber = 0;
        ///     UInt16 RevisionNumber = 0;
        ///     NmGetApiVersion(out majorNumber, out minorNumber, out BuildNumber, out RevisionNumber);
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
        [DllImport("NmApi.Dll")]
        public static extern void NmGetApiVersion(out UInt16 Major, out UInt16 Minor, out UInt16 Build, out UInt16 Revision);

        /// <summary><c>NmGetApiConfiguration</c>Return current API configuration parameters</summary> 
        /// <remarks>
        /// </remarks>
        /// <example>
        /// <code>
        ///    NM_API_CONFIGURATION apiConfig = new NM_API_CONFIGURATION();
        ///    apiConfig.Size = (ushort)System.Runtime.InteropServices.Marshal.SizeOf(typeof(NM_API_CONFIGURATION));
        ///    uint status = NetmonAPI.NmGetApiConfiguration(ref apiConfig);
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="ApiConfig">[out] Struct object for API to fill</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     NM_STATUS_API_VERSION_MISMATCHED: The version of NM_API_CONFIGURATION struct is different
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetApiConfiguration(ref NM_API_CONFIGURATION ApiConfig);

        /// <summary><c>NmApiInitialize</c>Overwrite default configuration.</summary> 
        /// <remarks>
        /// Caller needs to provide storage for NmApiConfiguration struct.
        /// </remarks>
        /// <example>
        /// <code>
        ///    NM_API_CONFIGURATION apiConfig = new NM_API_CONFIGURATION();
        ///    apiConfig.Size = (ushort)System.Runtime.InteropServices.Marshal.SizeOf(typeof(NM_API_CONFIGURATION));
        ///    uint status = NetmonAPI.NmGetApiConfiguration(ref apiConfig);
        ///    apiConfig.ThreadingMode = 0;
        ///    status = NetmonAPI.NmApiInitialize(ref apiConfig);
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="ApiConfig">[in] Caller specified API configuration parameter struct</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     NM_STATUS_API_VERSION_MISMATCHED: The version of NM_API_CONFIGURATION struct is different
        ///     ERROR_INVALID_STATE: Can not change API configuration
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmApiInitialize(ref NM_API_CONFIGURATION ApiConfig);

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
        ///     
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmApiClose();

        /// <summary><c>NmCloseHandle</c> Release the reference to the object by handle</summary> 
        /// <remarks>
        /// Callers need to close all the object handles returned from API after finish using them.
        /// </remarks>
        /// <exception>None</exception>
        /// <param name="hObjectHandle"> [in] Handle to the object to release </param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>Void</returns>
        [DllImport("NmApi.Dll")]
        public static extern void NmCloseHandle(IntPtr hObjectHandle);
        
        #endregion

        #region Capture Engine Operations

        /// <summary><c>NmOpenCaptureEngine</c>Open a capture engine</summary> 
        /// <remarks>
        /// 
        /// </remarks>
        /// <example>
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="phCaptureEngine">[out] The returned handle to the capture engine object on success</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_INVALID_STATE:     The operation is not available.
        ///     ERROR_ENOUGH_MEMORY: Fail to allocate memory for the object.
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmOpenCaptureEngine(out IntPtr phCaptureEngine);

        /// <summary><c>NmGetAdapterCount</c>Return number of the adapters that the capture engine can access</summary> 
        /// <remarks>
        /// 
        /// </remarks>
        /// <example>
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="ExitMode">[in] The callback function exit mode</param>
        /// <param name="hCaptureEngine">[in] The capture engine under query</param>
        /// <param name="ulCount">[out] The returned count of adapters</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetAdapterCount(IntPtr hCaptureEngine, out UInt32 ulCount);

        /// <summary><c>NmGetAdapter</c>Get adapter information from the capture engine</summary> 
        /// <remarks>
        /// Caller can use name, GUID etc. to select adapter to use.  The adapter index should be within the 
        /// Range returned by NmGetAdapterCount method.  Caller needs to provide the storage of the
        /// NmNicAdapterInfo struct.
        /// The fix sized name buffers in the NM_NIC_ADAPTER_INFO structure are marshal to System.char[]. 
        /// Use String nameStr = new String("e.g., whateverDefinedStructName.ConnectionName") to translate the char array to a string object.
        /// </remarks>
        /// <example>
        /// <code>
        ///    IntPtr hCaptureEngine = HandleReturnedByNmOpenCaptureEngine;
        ///    UInt32 ulIndex = 0;
        ///    NM_NIC_ADAPTER_INFO adapterInfo = new NM_NIC_ADAPTER_INFO();
        ///    adapterInfo.Size = (ushort)System.Runtime.InteropServices.Marshal.SizeOf(adapterInfo);
        ///    uint status = NetmonAPI.NmGetAdapter(hCaptureEngine, ulIndex, ref adapterInfo);
        ///    String nameStr = new String(adapterInfo.ConnectionName);
        /// </code>
        /// </example>
        /// <param name="ExitMode">[in] The callback function exit mode</param>
        ///
        /// <exception>None</exception>
        /// <param name="hCaptureEngine">[in] The handle of the capture engine object</param>
        /// <param name="ulIndex">[in] The index number of the adapter to retrieve</param>
        /// <param name="pNMAdapterInfo">[out] The returned adapter information struct</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     NM_STATUS_API_VERSION_MISMATCHED: The version of NM_NIC_ADAPTER_INFO struct is different
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetAdapter(IntPtr hCaptureEngine, UInt32 ulIndex, ref NM_NIC_ADAPTER_INFO pNMAdapterInfo);

        /// <summary><c>NmConfigAdapter</c>Configure the adapter with the frame indication callback and the caller context.</summary> 
        /// <remarks>
        /// The current callback function and context will overwrite the previous ones.  The adapter index number
        /// Must be in the range returned from NmGetAdapterCount method.
        /// </remarks>
        /// <example> Description
        /// <code>
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
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmConfigAdapter(IntPtr hCaptureEngine, UInt32 ulIndex, CaptureCallbackDelegate CallbackFunction, IntPtr pCallerContext, NmCaptureCallbackExitMode ExitMode);
        
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
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmStartCapture(IntPtr hCaptureEngine, UInt32 ulAdapterIndex, NmCaptureMode CaptureMode);

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
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmPauseCapture(IntPtr hCaptureEngine, UInt32 ulAdapterIndex);


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
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmResumeCapture(IntPtr hCaptureEngine, UInt32 ulAdapterIndex);

        /// <summary><c>NmStopCapture</c>Stop capture on given capture engine and adapter</summary> 
        /// <remarks>
        /// The frame indication callback is no longer invoked after this function returns.
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
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmStopCapture(IntPtr hCaptureEngine, UInt32 ulAdapterIndex);
        #endregion
      
        #region Parsing Functions
        
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
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_ENOUGH_MEMORY: Fail to create NPL parser object
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmLoadNplParser([MarshalAs(UnmanagedType.LPWStr)] String pFileName, NmNplParserLoadingOption ulFlags, ParserCallbackDelegate CallbackFunction, IntPtr pCallerContext, out IntPtr phNplParser);
        
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
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_ENOUGH_MEMORY: Fail to create frame parser configuration object.
        ///     ERROR_NOT_FOUND: not found specified NPL parser
        ///     
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmCreateFrameParserConfiguration(IntPtr hNplParser, ParserCallbackDelegate CallbackFunction, IntPtr pCallerContext, out IntPtr phFrameParserConfiguration);
 
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
        /// <param name="hFrameParserConfiguration">[in] The handle of the frame parser configuration object</param>
        /// <param name="pFilterString">[in] The text of the filter</param>
        /// <param name="ulFilterId">[out] The returned filter index in the frame parser</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified frame parser configuration
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmAddFilter(IntPtr hFrameParserConfiguration, [MarshalAs(UnmanagedType.LPWStr)] String pFilterString, out UInt32 ulFilterId);

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
        /// <param name="hFrameParserConfiguration">[in] The handle of the frame parser configuration object</param>
        /// <param name="pFieldString">[in] The fully qualified name string of the field</param>
        /// <param name="ulFieldId">[out] The returned field index in the frame parser</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified frame parser configuration
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmAddField(IntPtr hFrameParserConfiguration, [MarshalAs(UnmanagedType.LPWStr)] String pFieldString, out UInt32 ulFieldId);

        /// <summary><c>NmAddProperty</c>Add a property to the configuration.</summary> 
        /// <remarks>
        /// The property name should have scope prefix such as Conversation, Global, etc.  If not specified, 
        /// The frame property is the default scope.
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
        /// <param name="pName">[in] Fully qualified name of the property.</param>
        /// <param name="pulPropertyId">[out] Returned ID used to reference the property.</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_NOT_FOUND: not found specified frame parser configuration
        ///     ERROR_INVALID_PARAMETER: The specified property name is invalid
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmAddProperty(IntPtr hFrameParserConfiguration, [MarshalAs(UnmanagedType.LPWStr)] String pPropertyString, out UInt32 ulPropertyId);

        /// <summary><c>NmAddSequenceOrderConfig</c>Add protocol sequence order configurations</summary> 
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
        /// <param name="hFrameParserConfiguration">[in] The handle of the frame parser configuration object</param>
        /// <param name="SeqConfig">[in] Caller provided sequence configuration data</param>
        /// <param name="ulConfigId">[out] The configuration in the frame parser</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: NULL pointer
        ///     ERROR_NOT_ENOUGH_MEMORY: Fail to allocate memory to store the configuration.
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmAddSequenceOrderConfig(IntPtr hFrameParserConfiguration, ref NM_PROTOCOL_SEQUENCE_CONFIG SeqConfig, out UInt32 ulConfigId);

        /// <summary><c>NmConfigReassembly</c>Enable or disable reassembly</summary> 
        /// <remarks>
        /// </remarks>
        /// <example> 
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hFrameParserConfiguration">[in] The handle of the frame parser configuration object</param>
        /// <param name="Option">[in] Reassembly options</param>
        /// <param name="bEnable">[in] Action to take, enable or disable</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or option
        ///     ERROR_NOT_FOUND: not found specified frame parser configuration
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmConfigReassembly(IntPtr hFrameParserConfiguration, NmReassemblyConfigOption Option, Boolean bEnable);

        /// <summary><c>NmConfigConversation</c>Configure conversation options</summary> 
        /// <remarks>
        /// </remarks>
        /// <example> Description
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hFrameParserConfiguration">[in] The handle of the frame parser configuration object</param>
        /// <param name="Option">[in] conversation configuration options</param>
        /// <param name="bEnable">[in] Action to take, enable or disable</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or option
        ///     ERROR_NOT_FOUND: not found specified frame parser configuration
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmConfigConversation(IntPtr hFrameParserConfiguration, NmConversationConfigOption Option, Boolean bEnable);


        /// <summary><c>NmCreateFrameParser</c>Create frame parser with the given configuration</summary> 
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
        /// <param name="phParser">[out] The frame parser</param>
        /// <param name="OptimizeOption">[in] The optimization flag</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified frame parser configuration
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmCreateFrameParser(IntPtr hFrameParserConfiguration, out IntPtr phParser, NmFrameParserOptimizeOption OptimizeOption);


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
        /// <param name="StartDataTypeName">[in] The name of the data type that the created frame parser starts with</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified frame parser configuration
        /// </returns>
        /// 
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmConfigStartDataType(IntPtr hFrameParserConfiguration, [MarshalAs(UnmanagedType.LPWStr)] String StartDataTypeName);


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
        /// <param name="ulBufferLength">[in] The element length of the caller provided buffer</param>
        /// <param name="pBuffer">[out] The name of the data type that the created frame parser starts with</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified frame parser configuration
        /// </returns>
        /// 
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetStartDataType(IntPtr hFrameParserConfiguration, UInt32 ulBufferLength, char* pBuffer);


        #endregion

        #region Parsed Frame Operations
        /// <summary><c>NmParseFrame</c>Parse the raw netmon frame and return it in parsed format</summary> 
        /// <remarks>
        /// The parsed frame contains the frame information, filter state and enumeration of field.  When reassembly is
        /// Enabled, the last fragment of the payload completing the reassembly process and insert the reasembled raw frame.
        /// </remarks>
        /// <example>
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hFrameParser">[in] The handle of the frame parser used to parse the Given frame object</param>
        /// <param name="hRawFrame">[in] The handle of the target frame to parser</param>
        /// <param name="ulFrameNumber">[in] The frame number should be used in parsing process if enabled by option flag</param>
        /// <param name="Options">[in] See flag definition NmFrameParsingOption</param>
        /// <param name="phParsedFrame">[out] The handle to the result parsed frame object</param>
        /// <param name="phInsertedRawFrame">[out] the handle of inserted raw frame as the result of parsing, e.g., reassembly</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified frame parser or raw frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmParseFrame(IntPtr hFrameParser, IntPtr hRawFrame, UInt32 ulFrameNumber, NmFrameParsingOption Options, out IntPtr phParsedFrame, out IntPtr phInsertedRawFrame);

        
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
        /// <param name="hParser">[in] The handle of the frame parser used to parse the Given frame object</param>
        /// <param name="DataBuffer">[in] The pointer to the target data buffer</param>
        /// <param name="ulBufferLength">[in] length of the data buffer in previous parameter</param>
        /// <param name="ulFrameNumber">[in] The frame number should be used in parsing process if enabled by option flag</param>
        /// <param name="Options">[in] See flag definition NmFrameParsingOption</param>
        /// <param name="phParsedFrame">[out] The handle to the result parsed frame object</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified frame parser or raw frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmParseBuffer(IntPtr hParser, Byte[] DataBuffer, UInt32 ulBufferLength, UInt32 ulFrameNumber, NmFrameParsingOption Options, out IntPtr phParsedFrame);

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
        /// <param name="DataBuffer">[in] The pointer to the target data buffer</param>
        /// <param name="ulBufferLength">[in] length of the data buffer in previous parameter</param>
        /// <param name="ulMedia">[in] Media type of the target raw frame</param>
        /// <param name="ullTimeStamp">[in] Capture time stamp of the target raw frame</param>
        /// <param name="phRawFrame">[out] The handle to the result parsed frame object</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_ENOUGH_MEMORY: No space to build the new frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmBuildRawFrameFromBuffer(IntPtr DataBuffer, UInt32 ulBufferLength, UInt32 ulMedia, UInt64 ullTimeStamp, out IntPtr phRawFrame);

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
        /// <param name="FragmentationInfo">[out] Caller provided struct pointer</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame
        ///     ERROR_INSUFFICIENT_BUFFER: If the protocol name length is longer than the buffer in PNmReassemblyInfo struct
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFrameFragmentInfo(IntPtr hParsedFrame, ref NmFragmentationInfo FragmentationInfo);

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
        /// <param name="ulFilterCount">[out] number of filters of the given frame parser.  It is zero if return code is not success</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified frame parser
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFilterCount(IntPtr hFrameParser, out UInt32 ulFilterCount);

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
        /// <param name="bPassFilter">[out] The filter evaluation result.  TRUE means pass; FALSE means not pass</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmEvaluateFilter(IntPtr hParsedFrame, UInt32 ulFilterId, out Boolean bPassFilter);

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
        /// <param name="ulFieldCount">[out] The number of fields returned in parsed frame</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFieldCount(IntPtr hParsedFrame, out UInt32 ulFieldCount);

        /// <summary><c>NmGetParsedFieldInfo</c>Return the field information of a parsed frame specified by field index number</summary> 
        /// <remarks>
        /// The pointer to field is valid until the parsed frame containing the field is closed.
        /// </remarks>
        /// <example>
        /// <code>
        ///     NmParsedFieldInfo parsedDataField = new NmParsedFieldInfo();
        ///     parsedDataField.Size = (ushort)Marshal.SizeOf(typeof(NmParsedFieldInfo));
        ///     NetmonAPI.NmGetParsedFieldInfo(hParsedFrame, 0, 0, ref parsedDataField);
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hParsedFrame">[in] The handle of the parsed frame</param>
        /// <param name="ulFieldId">[in] The identify number of the field to get</param>
        /// <param name="ulOption">[in] The retrieve flag</param>
        /// <param name="pParsedFieldInfo">[out] The pointer to the parsed field</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetParsedFieldInfo(IntPtr hParsedFrame, UInt32 ulFieldId, UInt32 ulOption, ref NmParsedFieldInfo pParsedFieldInfo);

        /// <summary><c>NmGetFieldName</c>Return the name property of the parsed field specified by field id</summary> 
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
        /// <param name="ulBufferLength">[in] The element length of caller provided buffer length</param>
        /// <param name="pBuffer">[out] The caller provided buffer</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        ///     ERROR_INSUFFICIENT_BUFFER: If ulBufferLength is shorted than the name length
        /// </returns>
        [DllImport("NmApi.Dll", CharSet=CharSet.Unicode ) ]
        unsafe public static extern UInt32 NmGetFieldName(IntPtr hParsedFrame, UInt32 ulFieldId, NmParsedFieldNames RequestedName, UInt32 ulBufferLength, char* pBuffer);

        /// <summary><c>NmGetFieldOffsetAndSize</c>Return the offset and size of the field specified by field id</summary> 
        /// <remarks>
        /// The returned field size is in unit of byte
        /// </remarks>
        /// <example> Description
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
        /// <param name="ulFieldId">[in] The identify number of the field</param>
        /// <param name="ulFieldOffset">[out] The pointer to the returned field offset</param>
        /// <param name="ulFieldSize">[out] The pointer to the returned field size</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFieldOffsetAndSize(IntPtr hParsedFrame, UInt32 ulFieldId, out UInt32 ulFieldOffset, out UInt32 ulFieldSize);

        /// <summary><c>NmGetFieldValueNumber8Bit</c>Return 8-bit field value</summary> 
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
        /// <param name="ubNumber">[out] The value of the requested field</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFieldValueNumber8Bit(IntPtr hParsedFrame, UInt32 ulFieldId, out Byte ubNumber);

        /// <summary><c>NmGetFieldValueNumber16Bit</c>Return 16-bit field value</summary> 
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
        /// <param name="uiNumber">[out] The value of the requested field</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFieldValueNumber16Bit(IntPtr hParsedFrame, UInt32 ulFieldId, out UInt16 uiNumber);

        /// <summary><c>NmGetFieldValueNumber32Bit</c>Return 32-bit field value</summary> 
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
        /// <param name="ulNumber">[out] The value of the requested field</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFieldValueNumber32Bit(IntPtr hParsedFrame, UInt32 ulFieldId, out UInt32 ulNumber);

        /// <summary><c>NmGetFieldValueNumber64Bit</c>Return 64-bit field value</summary> 
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
        /// <param name="ullNumber">[out] The value of the requested field</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFieldValueNumber64Bit(IntPtr hParsedFrame, UInt32 ulFieldId, out UInt64 ullNumber);

        /// <summary><c>NmGetFieldValueByteArray</c>Return byte array field value in buffer</summary> 
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
        /// <param name="ulByteLength">[in] The length of the provided buffer in byte</param>
        /// <param name="pBuffer">[out] The value of the requested field</param>
        /// <param name="ulReturnLength">[out] The number of bytes actaully copied</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetFieldValueByteArray(IntPtr hParsedFrame, UInt32 ulFieldId, UInt32 ulByteLength, byte* pBuffer, out UInt32 ulReturnLength);

        /// <summary><c>NmGetFieldValueString</c>Return string field value in buffer</summary> 
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
        /// <param name="ulBufferLength">[in] The element length of the provided buffer</param>
        /// <param name="pValueBuffer">[out] The value of the requested field</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetFieldValueString(IntPtr hParsedFrame, UInt32 ulFieldId, UInt32 ulBufferLength, char* pValueBuffer);

        /// <summary><c>NmGetFieldInBuffer</c>Get the field in user provided buffer</summary> 
        /// <remarks>
        /// Only the content up to the buffer length is copied.  Caller may call NmGetFieldOffsetAndSize to get the size 
        /// Before calling this function with proper buffer length.
        /// </remarks>
        /// <example>
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hParsedFrame">[in] The handle of the target parsed frame</param>
        /// <param name="ulFieldId">[in] The identify number of the field</param>
        /// <param name="ulBufferLength">[in] The element length of caller provided buffer</param>
        /// <param name="pFieldBuffer">[out] caller provided buffer</param>
        /// <param name="ulReturnLength">[out] actual number of byte copied</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_NOT_FOUND: not found specified parsed frame or field
        ///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, data is not copied.
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetFieldInBuffer(IntPtr hParsedFrame, UInt32 ulFieldId, UInt32 ulBufferLength, byte* pFieldBuffer, out UInt32 ulReturnLength);

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
        /// <param name="hFrameParser">[in] Frame Parser Configuration Handle</param>
        /// <param name="ulCount">[out] Count of properties added to this frame configuration.</param>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle, wrong scope or NULL pointer
        ///     ERROR_NOT_FOUND: not found specified frame parser
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetRequestedPropertyCount(IntPtr hFrameParser, out UInt32 ulCount);

        /// <summary><c>NmGetPropertyInfo</c>Return info structure for a specific property by ID.</summary> 
        /// <remarks>
        /// None.
        /// </remarks>
        /// <example> This sample shows how to call the NmGetParsedPropertyInfo method.
        /// <code>
        ///     HANDLE hFrameParser;
        ///     ULONG PropertyId;
        ///     PNM_NPL_PROPERTY_INFO PropertyInfo;
        ///
        ///     PropertyInfo.Size = sizeof(NmPropertyInfo);
        ///     NmGetPropertyInfo(hFrameParser, PropertyId, &PropertyInfo);
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hFrameParserConfiguration">[in] Frame Parser Configuration Handle</param>
        /// <param name="ulPropertyId">[in] ID of the property returned from NmAddProperty</param>
        /// <param name="pInfo">[out] Count of properties added to this frame configuration.</param>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_NOT_FOUND: not found specified property
        ///     NM_STATUS_API_VERSION_MISMATCHED: NM_PARSED_PROPERTY_INFO version mismatch
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetPropertyInfo(IntPtr hFrameParser, UInt32 ulPropertyId, ref NM_NPL_PROPERTY_INFO pInfo);

        /// <summary><c>NmGetPropertyById</c>Return property value by ID.</summary> 
        /// <remarks>
        /// The Key for multi-storage properties or Index for array properties must not provided by both key array
        /// And property name represented by property ID added to the frame parser configuration.
        /// If no key to add, set ulKeyCount to zero.
        /// </remarks>
        /// <example>.
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hFrameParser">[in] Frame Parser Handle</param>
        /// <param name="ulPropertyId">[in] ID of the property returned from NmAddProperty</param>
        /// <param name="ulBufferSize">[in] Size of the buffer supplied in byte count.</param>
        /// <param name="pBuffer">[out] Buffer for returned data.</param>
        /// <param name="ulReturnLength">[out] Size of the data returned.</param>
        /// <param name="ulType">[out] Value type of the returned MVS property.</param>
        /// <param name="ulKeyCount">[in] Number of keys provided</param>
        /// <param name="pKeyArray">[in] key Array to look up in MVS and property group </param>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_NOT_FOUND: not found specified property
        ///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, data is not copied.
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetPropertyById(IntPtr hFrameParser, UInt32 ulPropertyId, UInt32 ulBufferSize, byte* pBuffer, out UInt32 ulReturnLength, out NmPropertyValueType ulType, UInt32 ulKeyCount, CNmPropertyStorageKey [] pKeyArray);

        /// <summary><c>NmGetPropertyByName</c>Return property value by Name.</summary> 
        /// <remarks>
        /// The property is not necessarily added to the frame parser configuration if a non-optimized frame parser is used.  In this case, the property id is not available and the
        /// The property name can be used.  The full qualified name must be used as to add the property to the frame parser configuration.
        /// The index or key must not provided by both name and key array. If no key to add, set ulKeyCount to zero.
        /// </remarks>
        /// <example>
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hFrameParser">[in] Frame Parser Handle</param>
        /// <param name="pPropertyName">[in] full qualified name of the property </param>
        /// <param name="ulBufferSize">[in] Size of the buffer supplied in byte count.</param>
        /// <param name="pBuffer">[out] Buffer for returned data.</param>
        /// <param name="pulReturnLength">[out] Size of the data returned.</param>
        /// <param name="ulType">[out] Value type of the returned MVS property.</param>
        /// <param name="ulKeyCount">[in] Number of keys provided</param>
        /// <param name="pKeyArray">[in] key Array to look up in MVS and property group </param>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_NOT_FOUND: not found specified property
        ///     ERROR_INSUFFICIENT_BUFFER: Not enough space in buffer, data is not copied.
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetPropertyByName(IntPtr hFrameParser, [MarshalAs(UnmanagedType.LPWStr)] String pPropertyName, UInt32 ulBufferSize, byte* pBuffer, out UInt32 pulReturnLength, out NmPropertyValueType ulType, UInt32 ulKeyCount, CNmPropertyStorageKey [] pKeyArray);

        #endregion

        #region Raw Frame Operations
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
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified raw frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetRawFrameLength(IntPtr hFrame, out UInt32 pulLength);

        /// <summary><c>NmGetRawFrame</c>Return raw frame</summary> 
        /// <remarks>
        /// The frame buffer is valid until the raw frame is closed.
        /// </remarks>
        /// <example>
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hFrame">[in] The handle of the target raw frame</param>
        /// <param name="ulLength">[in] Caller buffer length in byte element</param>
        /// <param name="pFrameBuffer"> [out] Buffer for raw frame data by caller</param>
        /// <param name="pulReturnLength">[out] Actual returned data length</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified raw frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetRawFrame(IntPtr hFrame, UInt32 ulLength, byte* pFrameBuffer, out UInt32 pulReturnLength);

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
        /// <param name="hRawFrame">[in] The handle of the target raw frame</param>
        /// <param name="ulFrameOffset">[in] Start offset to copy</param>
        /// <param name="ulBufferLength">[in] Caller buffer length, the Number of bytes to copy</param>
        /// <param name="pFrameBuffer">[out] Caller provided buffer</param>
        /// <param name="pulReturnLength">[out] Actual returned data length</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified raw frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetPartialRawFrame(IntPtr hRawFrame, UInt32 ulFrameOffset, UInt32 ulBufferLength, byte* pFrameBuffer, out UInt32 pulReturnLength);

        /// <summary><c>NmGetFrameMacType</c>Return MAC type of the frame</summary> 
        /// <remarks>
        /// </remarks>
        /// <example>
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hParsedFrame">[in] The handle of the parsed frame object</param>
        /// <param name="pulMacType">[out] The pointer to the MAC type of the frame</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFrameMacType(IntPtr hParsedFrame, out UInt32 pulMacType);


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
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified parsed frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFrameTimeStamp(IntPtr hParsedFrame, out UInt64 pTimeStamp);

        /// <summary><c>NmGetFrameCommentInfo</c>Return the frame comment title and description</summary>
        /// <remarks>
        /// If the buffer passed in is NULL, the buffer length arguments will indicate the required length.
        /// </remarks>
        /// <example>
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="hFrame"> [in] The handle of the parsed frame object></param>
        /// <param name="ulCommentTitleBufferLength"> [inout] The pointer to the actual byte length that corresponds to the title buffer</param>
        /// <param name="pCommentTitleBuffer"> [in] Caller supplied buffer to hold the comment title</param>
        /// <param name="ulCommentDescriptionBufferLength"> [inout] The pointer to the actual byte length that corresponds to the description buffer</param>
        /// <param name="pCommentDescriptionBuffer"> [in] Caller supplied buffer to hold the comment description</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_NOT_FOUND: Specified parsed frame not found
        ///     ERROR_INSUFFICIENT_BUFFER: If either of the supplied buffers is NULL
        ///     ERROR_EMPTY: Frame comment information was not found
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetFrameCommentInfo(IntPtr hFrame, ref UInt32 ulCommentTitleBufferLength, byte* pCommentTitleBuffer, ref UInt32 ulCommentDescriptionBufferLength, byte* pCommentDescriptionBuffer);

        #endregion

        #region Capture File Operations

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
        /// <param name="pFileName"> [in] The name of the file to create</param>
        /// <param name="ulSize"> [in] The caller specified maximum size of the file in byte.  The hard limit is 500 MByte</param>
        /// <param name="ulFlags"> [in] Specify the file modes, wrap-round or chain capture</param>
        /// <param name="phCaptureFile"> [out] The returned handle to the capture file object if successful</param>
        /// <param name="ulReturnSize"> [out] The actual size of the file in byte.</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: NULL pointer
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmCreateCaptureFile([MarshalAs(UnmanagedType.LPWStr)] String pFileName, UInt32 ulSize, NmCaptureFileFlag ulFlags, out IntPtr phCaptureFile, out UInt32 ulReturnSize);

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
        /// <param name="pFileName"> [in] The name of the file to open</param>
        /// <param name="phCaptureFile"> [out]The returned handle of the capture file object if successful</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: NULL pointer
        ///     ERROR_NOT_FOUND: not found specified file
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmOpenCaptureFile([MarshalAs(UnmanagedType.LPWStr)] String pFileName, out IntPtr phCaptureFile);

        /// <summary><c>NmOpenCaptureFileInOrder</c> Open a Netmon capture file to read</summary> 
        /// <remarks>
        /// The frame in file are in the order of the sequence specified in the frame parser parameter
        /// The file is read only. Close capture file by calling NmCloseObjHandle method.
        /// </remarks>
        /// <example> 
        /// <code>
        /// </code>
        /// </example>
        ///
        /// <exception>None</exception>
        /// <param name="pFileName"> [in] The name of the file to open</param>
        /// <param name="pOrderParser"> [in]The struct containing the frame parser configured with sequnece order instructions</param>
        /// <param name="phCaptureFile"> [out]The returned handle of the capture file object if successful</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: NULL pointer
        ///     ERROR_NOT_FOUND: not found specified file
        ///     ERROR_INVALID_PARAMETER: frame parser does not have sequence configuration.
        ///     ERROR_NOT_ENOUGH_MEMORY: not enough memory to build required objects.
        ///     NM_STATUS_API_VERSION_MISMATCHED: PNM_ORDER_PARSER_PARAMETER version does not match.
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmOpenCaptureFileInOrder([MarshalAs(UnmanagedType.LPWStr)] String pFileName, ref NM_ORDER_PARSER_PARAMETER pOrderParser, out IntPtr phCaptureFile);

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
        /// <param name="hCaptureFile"> [in] The destination capture file for the frame</param>
        /// <param name="hFrame"> [in] The handle of the frame to add</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified file or frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmAddFrame(IntPtr hCaptureFile, IntPtr hFrame);


        /// <summary><c>NmGetFrameCount</c> Get frame count in the specified capture file</summary> 
        /// <remarks>
        /// 
        /// </remarks>
        /// <exception>None</exception>
        /// <param name="hCaptureFile"> [in] The target capture file under query</param>
        /// <param name="hFrameCount"> [out] Return frame count</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified capture file
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFrameCount(IntPtr hCaptureFile, out UInt32 hFrameCount);

        /// <summary><c>NmGetFrame</c> Get frame by number from the specified capture file.</summary> 
        /// <remarks>
        /// The frame number is the index number in the capture file.
        /// </remarks>
        /// <exception>None</exception>
        /// <param name="hCaptureFile"> [in] Handle to the capture file</param>
        /// <param name="ulFrameNumber"> [in] Frame number in the capture file to retrieve</param>
        /// <param name="phFrame"> [out] The returned handle to the raw frame object if successful</param>
        /// <permission cref="System.Security.PermissionSet">Everyone can access this method.</permission>
        /// <returns>
        ///     ERROR_SUCCESS: Frame handle is valid
        ///     ERROR_BAD_ARGUMENTS: Invalid handle
        ///     ERROR_NOT_FOUND: not found specified capture file or frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        public static extern UInt32 NmGetFrame(IntPtr hCaptureFile, UInt32 ulFrameNumber, out IntPtr phFrame);
        #endregion

        #region NPL Property Extension

        /// <summary><c>NmGetTopConversation</c>Return the top level conversation and protocol name.</summary> 
        /// <remarks>
        /// The protocol name length is returned to caller.  So if the provided buffer is not enough, caller
        /// Can call again with the proper sized buffer.
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
        ///     ERROR_NOT_FOUND: not found specified parsed frame
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetTopConversation(IntPtr hParsedFrame, UInt32 ulBufferESize, char* pProtocolName, out UInt32 pulProtocolNameLength, out UInt32 pulConversationID);

        /// <summary><c>NmGetParentConversation</c>Return parent conversation information of the given conversation.</summary> 
        /// <remarks>
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
        /// <param name="myParsedFrame">[in] Parsed Frame</param>
        /// <param name="ulConversationID">[in] ID of the Conversation you want the parent of.</param>
        /// <param name="ulBufferESize">[in] Buffer size for the Parent protocol name in WCHAR count.</param>
        /// <param name="pParentProtocolName">[out] Buffer for the Parent Protocol Name. </param>
        /// <param name="ulParentProtocolNameLength">[out] Returned Length of Parent Protocol Name in WCHAR.</param>
        /// <param name="ulParentConversationID">[out] Size of the for protocol name.</param>
        /// <returns>
        ///     ERROR_SUCCESS:
        ///     ERROR_BAD_ARGUMENTS: Invalid handle or NULL pointer
        ///     ERROR_INSUFFICIENT_BUFFER: Insufficient buffer space
        ///     ERROR_NOT_FOUND: not found specified frame parser
        /// </returns>
        [DllImport("NmApi.Dll")]
        unsafe public static extern UInt32 NmGetParentConversation(IntPtr hParsedFrame, UInt32 ulConversationId, UInt32 ulBufferESize, char* pParentProtocolNameBuffer, out UInt32 ulParentProtocolNameLength, out UInt32 ulParentConversationID);

        #endregion
    }
}
