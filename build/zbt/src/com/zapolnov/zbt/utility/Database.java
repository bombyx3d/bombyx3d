/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
package com.zapolnov.zbt.utility;

import java.io.File;
import java.util.Arrays;
import java.util.concurrent.ConcurrentNavigableMap;
import org.mapdb.DB;
import org.mapdb.DBMaker;

public class Database
{
    public final static String FILE_NAME = "database";

    public static final String OPTION_GENERATOR_NAME = "Generator";
    public static final String OPTION_CMAKE_BUILD_TYPE = "CMakeBuildType";
    public static final String OPTION_CMAKE_BUILD_TOOL = "CMakeBuildTool";
    public static final String PROJECT_OPTION_FORMAT = "ProjectOption:%s";

    private final static String INPUT_FILES_TABLE = "<InputFiles>";
    private final static String INPUT_FILES_OPTIONS_HASHES_TABLE = "<InputFilesOptionsHashes>";
    private final static String OUTPUT_FILES_TABLE = "<OutputFiles>";
    private final static String OPTIONS_TABLE = "<Options>";

    public final File directory;
    protected DB db;

    public Database(File directory)
    {
        this.directory = directory;
        db = DBMaker.newFileDB(new File(directory, FILE_NAME))
            .closeOnJvmShutdown()
            .make();
    }

    public void commit()
    {
        db.commit();
    }

    public void rollback()
    {
        db.rollback();
    }

    public void rollbackSafe()
    {
        try {
            rollback();
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }

    public void close()
    {
        if (db != null) {
            db.close();
            db = null;
        }
    }

    public String getOption(String key)
    {
        try {
            ConcurrentNavigableMap<String, String> table = db.getTreeMap(OPTIONS_TABLE);
            return table.get(key);
        } catch (Throwable t) {
            t.printStackTrace();
        }
        return null;
    }

    public void setOption(String key, String value)
    {
        ConcurrentNavigableMap<String, String> table = db.getTreeMap(OPTIONS_TABLE);
        table.put(key, value);
    }

    public boolean didInputFileChange(File file)
    {
        if (!file.exists())
            return true;

        try {
            ConcurrentNavigableMap<String, Long> table = db.getTreeMap(INPUT_FILES_TABLE);

            String path = Utility.getCanonicalPath(file);
            Long expectedLastModificationTime = table.get(path);
            long actualLastModificationTime = file.lastModified();

            if (expectedLastModificationTime != null && expectedLastModificationTime == actualLastModificationTime)
                return false;

            table.put(path, actualLastModificationTime);
        } catch (Throwable t) {
            t.printStackTrace();
        }

        return true;
    }

    public boolean didInputFileChange(File file, byte[] optionsHash)
    {
        boolean result = false;

        if (!file.exists())
            return true;

        try {
            String path = Utility.getCanonicalPath(file);

            ConcurrentNavigableMap<String, byte[]> hashesTable = db.getTreeMap(INPUT_FILES_OPTIONS_HASHES_TABLE);
            byte[] previousHash = hashesTable.get(path);
            if (previousHash == null || !Arrays.equals(optionsHash, previousHash)) {
                hashesTable.put(path, optionsHash);
                result = true;
            }

            ConcurrentNavigableMap<String, Long> table = db.getTreeMap(INPUT_FILES_TABLE);
            Long expectedLastModificationTime = table.get(path);
            long actualLastModificationTime = file.lastModified();
            if (expectedLastModificationTime == null || expectedLastModificationTime != actualLastModificationTime) {
                table.put(path, actualLastModificationTime);
                result = true;
            }

            return result;
        } catch (Throwable t) {
            t.printStackTrace();
            return true;
        }
    }

    public boolean didOutputFileChange(File file, byte[] md5)
    {
        try {
            ConcurrentNavigableMap<String, byte[]> table = db.getTreeMap(OUTPUT_FILES_TABLE);

            String path = Utility.getCanonicalPath(file);
            byte[] previousMd5 = table.get(path);
            if (previousMd5 != null && Arrays.equals(md5, previousMd5))
                return false;

            table.put(path, md5);
        } catch (Throwable t) {
            t.printStackTrace();
        }

        return true;
    }
}
