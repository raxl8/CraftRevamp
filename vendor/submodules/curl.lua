return {
    include = function()
        includedirs "include"
		defines { 'ssize_t=__int64', '_SSIZE_T_DEFINED=1', '_U_=', 'NGHTTP2_STATICLIB' }
    end,
    project = function()
        kind "SharedLib"
        language "C"

        includedirs "lib"
        files "lib/**.c"

        submodules { "nghttp2", "mbedtls" }

        defines { 'BUILDING_LIBCURL', 'USE_IPV6', 'CURL_DISABLE_TFTP', 'CURL_DISABLE_FTP', 'CURL_DISABLE_LDAP', 'CURL_DISABLE_TELNET',
				  'CURL_DISABLE_DICT', 'CURL_DISABLE_RTSP', 'CURL_DISABLE_POP3', 'CURL_DISABLE_IMAP', 'CURL_DISABLE_SMTP',
				  'CURL_DISABLE_RTMP', 'CURL_DISABLE_GOPHER', 'CURL_DISABLE_SMB', 'USE_IPV6', 'USE_NGHTTP2', 'USE_MBEDTLS' }

        if os.istarget('windows') then
            defines 'USE_WINDOWS_SSPI'
        end
    end
}
