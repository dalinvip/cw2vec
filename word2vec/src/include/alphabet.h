#ifndef _ALPHABET_
#define _ALPHABET_


/*
 please check to ensure that m_size not exceeds the upbound of int32_t
 */

/*
  This class serializes feature from string to int32_t.
  Index starts from 0.
*/

/**
 * The basic class of quark class.
 *  @param  std::string        String class name to be used.
 *  @param  int32_t         ID class name to be used.
 *  @author Naoaki Okazaki
 */
#include <unordered_map>
#include <vector>
#include <string>
#include <assert.h>
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

struct Cmp {
	std::string word;
	int32_t freq;
};

bool sort_by_freq(const Cmp &v1, const Cmp &v2)
{
	return v1.freq > v2.freq;
}

class basic_quark {
  public:
    typedef unordered_map<std::string, int32_t> StringToId;
    typedef map<std::string, int32_t> StringToId_Order;
    typedef std::vector<std::string> IdToString;
    typedef std::vector<int64_t> IdToFreq;

    StringToId m_string_to_id;
    IdToString m_id_to_string;
    IdToFreq   m_id_to_freq;
    int32_t m_size;
    int64_t m_max_freq;
    int64_t m_allword_count;

  protected:
    int32_t m_max_size;
    int32_t m_reduce_threshold;

  public:
    /**
     * Construct.
     */
    basic_quark() {
        clear();
        m_reduce_threshold = 1;
        m_max_size = 0;
    }

    /**
     * Destruct.
     */
    virtual ~basic_quark() {
    }

    inline void setCapacity(int max_size) {
        m_max_size = max_size;
    }


    /**
     * Map a string to its associated ID.
     *  If string-to-int32_teger association does not exist, allocate a new ID.
     *  @param  str         String value.
     *  @return           Associated ID for the string value.
     */
    int32_t from_string(const std::string& str) const {
        StringToId::const_iterator it = m_string_to_id.find(str);
        if (it != m_string_to_id.end()) {
            return it->second;
        } else {
            return -1;
        }
    }


    /**
     * Convert ID value int32_to the associated string value.
     *  @param  qid         ID.
     *  @param  def         Default value if the ID was out of range.
     *  @return           String value associated with the ID.
     */
    const std::string& from_id(const int32_t& qid, const std::string& def = "") const {
        if (qid < 0 || m_size <= qid) {
            return def;
        } else {
            return m_id_to_string[qid];
        }
    }



    /**
     * Convert string value int32_to the associated ID value.
     *  @param  str         String value.
     *  @return           ID if any, otherwise -1.
     */
    int32_t add_string(const std::string& str, int64_t freq = 1) {
        StringToId::const_iterator it = m_string_to_id.find(str);
        if (it != m_string_to_id.end()) {
            int32_t qid = it->second;
            m_id_to_freq[qid] = m_id_to_freq[qid] + freq;
            if(m_id_to_freq[qid] > m_max_freq) m_max_freq = m_id_to_freq[qid];
            m_allword_count += freq;
            return it->second;
        } else {
            int32_t newid = m_size;
            m_id_to_string.push_back(str);
            m_id_to_freq.push_back(freq);
            if(m_size == 0) m_max_freq = freq;
            m_allword_count += freq;
            m_string_to_id.insert(std::pair<std::string, int32_t>(str, newid));
            m_size++;
            if (m_size >= m_max_size) {
                reduce();
            }
            return newid;
        }
    }

    void clear() {
        m_string_to_id.clear();
        m_id_to_string.clear();
        m_id_to_freq.clear();
        m_size = 0;
        m_max_freq = 0;
        m_allword_count = 0;
    }

    /**
     * Get the number of string-to-id associations.
     *  @return           The number of association.
     */
    size_t size() const {
        return m_size;
    }

    void prune(int threshold) {
        if (m_reduce_threshold >= threshold) {
            return;
        }
        m_reduce_threshold = threshold;
        reduce();
    }

  protected:
    void reduce() {
        //std::cout << "Reaching max size, reducing low-frequency items" << std::endl;
        //std::cout << "Current Size: " << m_size << std::endl;
        StringToId tmp_string_to_id;

        for (int32_t idx = 0; idx < m_size; idx++) {
            if (m_id_to_freq[idx] < m_reduce_threshold) continue;
            tmp_string_to_id[m_id_to_string[idx]] = m_id_to_freq[idx];
        }
	
        clear();
		StringToId_Order tmp_string_to_id_order;
		std::vector<Cmp> vector_cmp;
		sort_freq(tmp_string_to_id, vector_cmp);

		for (int32_t i = 0; i < vector_cmp.size(); i++) {
			add_string(vector_cmp[i].word, vector_cmp[i].freq);
		}

        //std::cout << "Remain Size: " << m_size << std::endl;
        std::cout << "(" << m_size  << ")" << " ";
        std::cout.flush();
        m_reduce_threshold++;
    }

	void sort_freq(StringToId &tmp_string_to_id, std::vector<Cmp> &vector_cmp) {
		
		vector_cmp.clear();
		vector_cmp.resize(tmp_string_to_id.size());
		int64_t id = 0;
		for (StringToId::const_iterator it = tmp_string_to_id.begin(); it != tmp_string_to_id.end(); it++) {
			vector_cmp[id].word = it->first;
			vector_cmp[id].freq = it->second;
			id++;
		}

		std::sort(vector_cmp.begin(), vector_cmp.end(), sort_by_freq);
	}
};

typedef basic_quark alphabet;


#endif

