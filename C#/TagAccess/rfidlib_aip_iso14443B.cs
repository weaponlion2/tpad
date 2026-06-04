using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;


namespace WindowsApplication2
{
    class rfidlib_aip_iso14443B
    {

#if UNICODE
        [DllImport("rfidlib_aip_iso14443B.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int ISO14443B_ParseTagDataReport(UIntPtr hTagReport,
                                        ref  UInt32 aip_id,
                                        ref  UInt32 tag_id,
                                        ref  UInt32 ant_id,
                                        ref  UInt32 metaFlags,
                                          byte[] tagData,
                                        ref  UInt32 tagDataLen/* IN:max size of buffer for writing ,OUT:bytes written to buffer */);

        [DllImport("rfidlib_aip_iso14443B.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UIntPtr ISO14443B_CreateInvenParam(UIntPtr hInvenParamSpecList,
                                                            byte AntennaID,/* By default set to 0,apply to all antenna */
                                                            byte AFI,
                                                            byte SlotNum
                                                            );
#else
        [DllImport("rfidlib_aip_iso14443B.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern int ISO14443B_ParseTagDataReport(UIntPtr hTagReport,
                                        ref  UInt32 aip_id,
                                        ref  UInt32 tag_id,
                                        ref  UInt32 ant_id,
                                        ref  UInt32 metaFlags,
                                          byte[] tagData,
                                        ref  UInt32 tagDataLen/* IN:max size of buffer for writing ,OUT:bytes written to buffer */);

        [DllImport("rfidlib_aip_iso14443B.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern UIntPtr ISO14443B_CreateInvenParam(UIntPtr hInvenParamSpecList,
                                                            byte AntennaID,/* By default set to 0,apply to all antenna */
                                                            byte AFI,
                                                            byte SlotNum
                                                            );

#endif



    }
}
