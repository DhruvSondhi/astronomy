/*=============================================================================
Copyright 2019 Pranam Lashkari <plashkari628@gmail.com>
Copyright 2020 Gopi Krishna Menon <krishnagopi487.github@outlook.com>

Distributed under the Boost Software License, Version 1.0. (See accompanying
file License.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef BOOST_ASTRONOMY_IO_DEFAULT_HDUS_HPP
#define BOOST_ASTRONOMY_IO_DEFAULT_HDUS_HPP


#include<boost/astronomy/io/header.hpp>
#include<boost/astronomy/io/primary_hdu.hpp>
#include<boost/astronomy/io/binary_table.hpp>
#include<boost/astronomy/io/ascii_table.hpp>
#include<boost/variant.hpp>



namespace boost { namespace astronomy {namespace io {

    /**
     * @brief Visitor for writing the Header and Data of all types of HDU's
     * @tparam FileWriter Type of writer object to write data to
    */
    template<typename FileWriter>
    struct fits_writer_visitor :public boost::static_visitor<> {
    private:
        FileWriter& writer;
    public:

        /**
         * @brief Constructs a visitor object with an associated file_writer to write data to
         * @param[in,out] file_writer Object for facilitating the writing of data to the file
        */
        fits_writer_visitor(FileWriter& file_writer) :writer(file_writer) {}

        /**
         * @breif Special case ( Not to do anything )
        */
        void operator()(boost::blank) {}

        /**
         * @brief Calls the write_to function of given hdu to write data to the file_writer
         * @tparam Hdu  Type of HDU
         * @param[in] hdu Hdu object to call write_to
        */
        template<typename Hdu>
        void operator()(Hdu& hdu) { hdu.write_to(writer); }
    };

    /**
     * @brief           Contains factory methods for constructing different type of HDU's
     * @author          Gopi Krishna Menon
     */
    template<typename CardPolicy,typename AsciiConverter,typename BinaryConverter>
    struct default_hdu_manager{

        typedef boost::variant<
            boost::blank,
            basic_primary_hdu<CardPolicy,BinaryConverter>,   
            basic_binary_table_extension<CardPolicy,BinaryConverter>,
            basic_ascii_table<CardPolicy,AsciiConverter>

        >  Extension;

        template<typename FileWriter>
        using writer_visitor = fits_writer_visitor<FileWriter>;
        typedef header<CardPolicy> header_type;

        /**
         * @brief   Constructs an appropriate HDU from header information and data provided
         * @param[in] hdu_header Header associated with some perticular HDU
         * @param[in] data_buffer Data associated with some perticular HDU
        */
        static Extension construct_hdu(header<CardPolicy>& hdu_header, const std::string& data_buffer = "") {
            if (hdu_header.contains_keyword("SIMPLE")) {
                // ITS PRIMARY HDU
                return generate_primary_hdu(hdu_header, data_buffer);
            }
            else {
                // FOR ALL EXTENSION HDU's
                return generate_extension_hdu(hdu_header, data_buffer);
            }
        }

        /**
         * @brief   Constructs an extension HDU from header information and data provided
         * @param[in] hdu_header Header associated with some extension HDU
         * @param[in] data_buffer Data associated with some extension HDU
        */
        static Extension generate_extension_hdu(header<CardPolicy>& hdu_header, const std::string& data_buffer) {
            std::string extension_name = hdu_header.template value_of<std::string>("XTENSION");
            if (extension_name == "TABLE") { return basic_ascii_table<CardPolicy, AsciiConverter>(hdu_header, data_buffer); }
            else if (extension_name == "BINTABLE") { return basic_binary_table_extension<CardPolicy,BinaryConverter>(hdu_header, data_buffer); }
            else { return boost::blank{}; }
        }

        /**
         * @brief   Construct a primary hdu from header and data provided as argument
         * @param[in] prime_header Header associated with primaru HDU
         * @param[in] buffer Data associated with primary HDU
        */
        static Extension generate_primary_hdu(header<CardPolicy>& prime_header, const std::string& buffer) {
            Extension prime_hdu;
            prime_hdu = basic_primary_hdu<CardPolicy,BinaryConverter>(prime_header, buffer);
            return prime_hdu;
        }
    };
        }
    }
}
#endif
